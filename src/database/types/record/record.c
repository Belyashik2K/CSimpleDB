#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../record/record.h"
#include "../query/query.h"

#define DEFINE_INIT_FUNC(NAME, FIELD, TYPE, FACTORY)                      \
    static int initialize##NAME(Record *record, char *value, char *key) {   \
        TYPE *tmp = FACTORY(value, key);                                  \
        if (!tmp) return 0;                                               \
        record->FIELD = *tmp;                                             \
        return 1;                                                         \
    }

#define DEFINE_PRINT_FUNC(NAME, FIELD)                                    \
    static int print##NAME(Record *record) {                              \
        printf("%s", record->FIELD.toString(&record->FIELD));             \
        return 1;                                                         \
    }

#define DEFINE_VALIDATE_FUNC(NAME, KEY_STR, FACTORY)                      \
    int validate##NAME(char *value) {                                     \
        return FACTORY(value, KEY_STR) != NULL;                           \
    }

#define DEFINE_ALL_FUNCS(NAME, KEY_STR, FIELD, TYPE, FACTORY) \
    DEFINE_INIT_FUNC(NAME, FIELD, TYPE, FACTORY)              \
    DEFINE_PRINT_FUNC(NAME, FIELD)                            \
    DEFINE_VALIDATE_FUNC(NAME, KEY_STR, FACTORY)

DEFINE_ALL_FUNCS(GeoId, "geo_id", geo_id, CustomInt, intFactory)
DEFINE_ALL_FUNCS(GeoPos, "geo_pos", geo_pos, CustomString, stringFactory)
DEFINE_ALL_FUNCS(MeaDate, "mea_date", mea_date, Date, dateFactory)
DEFINE_ALL_FUNCS(Level, "level", level, CustomInt, intFactory)
DEFINE_ALL_FUNCS(Sunrise, "sunrise", sunrise, Time, timeFactory)
DEFINE_ALL_FUNCS(Sundown, "sundown", sundown, Time, timeFactory)
DEFINE_ALL_FUNCS(Weather, "weather", weather, Weather, weatherFactory)

typedef int (*init_func)(Record *, char *, char *);

typedef int (*print_func)(Record *);

typedef int (*validate_func)(char *);

typedef struct {
    const char *key;
    init_func initialize;
    print_func print;
    validate_func validate;
} KeyMap;

static KeyMap keyMappings[] = {
    {"geo_id", initializeGeoId, printGeoId, validateGeoId},
    {"geo_pos", initializeGeoPos, printGeoPos, validateGeoPos},
    {"mea_date", initializeMeaDate, printMeaDate, validateMeaDate},
    {"level", initializeLevel, printLevel, validateLevel},
    {"sunrise", initializeSunrise, printSunrise, validateSunrise},
    {"sundown", initializeSundown, printSundown, validateSundown},
    {"weather", initializeWeather, printWeather, validateWeather}
};

int printKey(const char *key, Record *record) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return keyMappings[i].print(record);
        }
    }
    return 0;
}

int validateKey(char *key) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return 1;
        }
    }
    return 0;
}

int validateValue(char *key, char *value) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return keyMappings[i].validate(value);
        }
    }
    return 0;
}

static int processKey(char *key, char *value, Record *record, int seen[7]) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            if (seen[i]) return 0;
            seen[i] = 1;
            return keyMappings[i].initialize(record, value, key);
        }
    }
    return 0;
}

int compareMeaDate(Date *self, char *other, const ComparisonOptionEnum option) {
    return self->compare(self, other, option);
}

Record *recordFactory(Query *query) {
    Record *record = malloc(sizeof(Record));
    if (!record)
        return NULL;

    int seen[7] = {0};
    for (int i = 0; i < query->field_count; i++) {
        const QueryField field = query->fields[i];
        if (!processKey(field.field, field.value, record, seen)) {
            free(record);
            return NULL;
        }
    }

    for (int i = 0; i < 7; i++) {
        if (!seen[i]) {
            free(record);
            return NULL;
        }
    }

    return record;
}
