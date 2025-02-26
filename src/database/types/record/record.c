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
    DEFINE_VALIDATE_FUNC(NAME, KEY_STR, FACTORY) \

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

typedef int (*compare_func)(Record *, char *, ComparisonOptionEnum);

typedef int (*update_func)(Record *, char *);

typedef char *(*toString_func)(void *);

typedef struct {
    const char *key;
    init_func initialize;
    print_func print;
    validate_func validate;
    compare_func compare;
    update_func update;
    toString_func toString;
} KeyMap;

int compareGeoId(Record *record, char *other, ComparisonOptionEnum option) {
    return record->geo_id.compare(&record->geo_id, other, option);
}

int compareGeoPos(Record *record, char *other, ComparisonOptionEnum option) {
    return record->geo_pos.compare(&record->geo_pos, other, option);
}

int compareMeaDate(Record *record, char *other, ComparisonOptionEnum option) {
    return record->mea_date.compare(&record->mea_date, other, option);
}

int compareLevel(Record *record, char *other, ComparisonOptionEnum option) {
    return record->level.compare(&record->level, other, option);
}

int compareSunrise(Record *record, char *other, ComparisonOptionEnum option) {
    return record->sunrise.compare(&record->sunrise, other, option);
}

int compareSundown(Record *record, char *other, ComparisonOptionEnum option) {
    return record->sundown.compare(&record->sundown, other, option);
}

int compareWeather(Record *record, char *other, ComparisonOptionEnum option) {
    return record->weather.compare(&record->weather, other, option);
}

int updateGeoId(Record *record, char *newValue) {
    return record->geo_id.update(&record->geo_id, newValue);
}

int updateGeoPos(Record *record, char *newValue) {
    return record->geo_pos.update(&record->geo_pos, newValue);
}

int updateMeaDate(Record *record, char *newValue) {
    return record->mea_date.update(&record->mea_date, newValue);
}

int updateLevel(Record *record, char *newValue) {
    return record->level.update(&record->level, newValue);
}

int updateSunrise(Record *record, char *newValue) {
    return record->sunrise.update(&record->sunrise, newValue);
}

int updateSundown(Record *record, char *newValue) {
    return record->sundown.update(&record->sundown, newValue);
}

int updateWeather(Record *record, char *newValue) {
    return record->weather.update(&record->weather, newValue);
}

char *getGeoIdStringRepresentation(Record *record) {
    return record->geo_id.toString(&record->geo_id);
}

char *getGeoPosStringRepresentation(Record *record) {
    return record->geo_pos.toString(&record->geo_pos);
}

char *getMeaDateStringRepresentation(Record *record) {
    return record->mea_date.toString(&record->mea_date);
}

char *getLevelStringRepresentation(Record *record) {
    return record->level.toString(&record->level);
}

char *getSunriseStringRepresentation(Record *record) {
    return record->sunrise.toString(&record->sunrise);
}

char *getSundownStringRepresentation(Record *record) {
    return record->sundown.toString(&record->sundown);
}

char *getWeatherStringRepresentation(Record *record) {
    return record->weather.toString(&record->weather);
}

static KeyMap keyMappings[] = {
    {"geo_id", initializeGeoId, printGeoId, validateGeoId, compareGeoId, updateGeoId, getGeoIdStringRepresentation},
    {"geo_pos", initializeGeoPos, printGeoPos, validateGeoPos, compareGeoPos, updateGeoPos, getGeoPosStringRepresentation},
    {"mea_date", initializeMeaDate, printMeaDate, validateMeaDate, compareMeaDate, updateMeaDate, getMeaDateStringRepresentation},
    {"level", initializeLevel, printLevel, validateLevel, compareLevel, updateLevel, getLevelStringRepresentation},
    {"sunrise", initializeSunrise, printSunrise, validateSunrise, compareSunrise, updateSunrise, getSunriseStringRepresentation},
    {"sundown", initializeSundown, printSundown, validateSundown, compareSundown, updateSundown, getSundownStringRepresentation},
    {"weather", initializeWeather, printWeather, validateWeather, compareWeather, updateWeather, getWeatherStringRepresentation}
};

int printKey(const char *key, Record *record) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return keyMappings[i].print(record);
        }
    }
    return 0;
}

char *getFieldStringRepresentation(const char *key, Record *record) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return keyMappings[i].toString(record);
        }
    }
    return NULL;
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

int isSatisfiedByCondition(Record *record, Condition *condition) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(condition->field, keyMappings[i].key) == 0) {
            return keyMappings[i].compare(record, condition->value, condition->comparison->operator);
        }
    }
    return 0;
}

int updateRecord(Record *record, QueryField *field) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(field->field, keyMappings[i].key) == 0) {
            return keyMappings[i].update(record, field->value);
        }
    }
    return 0;
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
