#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../record/record.h"

#include <stdio.h>

#include "../query/query.h"

static int initializeGeoId(Record *record, char *value, char *key) {
    CustomInt *geoID = intFactory(value, key);
    if (!geoID) return 0;
    record->geo_id = *geoID;
    return 1;
}

static int printGeoId(Record *record) {
    printf(record->geo_id.toString(&record->geo_id));
    return 1;
}

static int printGeoPos(Record *record) {
    printf(record->geo_pos.toString(&record->geo_pos));
    return 1;
}

static int printMeaDate(Record *record) {
    printf(record->mea_date.toString(&record->mea_date));
    return 1;
}

static int printLevel(Record *record) {
    printf(record->level.toString(&record->level));
    return 1;
}

static int printSunrise(Record *record) {
    printf(record->sunrise.toString(&record->sunrise));
    return 1;
}

static int printSundown(Record *record) {
    printf(record->sundown.toString(&record->sundown));
    return 1;
}

static int printWeather(Record *record) {
    printf(record->weather.toString(&record->weather));
    return 1;
}

static int initializeGeoPos(Record *record, char *value, char *key) {
    CustomString *geoPos = stringFactory(value, key);
    if (!geoPos) return 0;
    record->geo_pos = *geoPos;
    return 1;
}

static int initializeMeaDate(Record *record, char *value, char *key) {
    Date *meaDate = dateFactory(value, key);
    if (!meaDate) return 0;
    record->mea_date = *meaDate;
    return 1;
}

static int initializeLevel(Record *record, char *value, char *key) {
    CustomInt *level = intFactory(value, key);
    if (!level) return 0;
    record->level = *level;
    return 1;
}

static int initializeSunrise(Record *record, char *value, char *key) {
    Time *sunrise = timeFactory(value, key);
    if (!sunrise) return 0;
    record->sunrise = *sunrise;
    return 1;
}

static int initializeSundown(Record *record, char *value, char *key) {
    Time *sundown = timeFactory(value, key);
    if (!sundown) return 0;
    record->sundown = *sundown;
    return 1;
}

static int initializeWeather(Record *record, char *value, char *key) {
    Weather *weather = weatherFactory(value, key);
    if (!weather) return 0;
    record->weather = *weather;
    return 1;
}

typedef int (*init_func)(Record *, char *, char *);
typedef int (*print_func)(Record *);

typedef struct {
    const char *key;
    init_func func;
    print_func print;
} KeyMap;

static KeyMap keyMappings[] = {
    {"geo_id", initializeGeoId, printGeoId},
    {"geo_pos", initializeGeoPos, printGeoPos},
    {"mea_date", initializeMeaDate, printMeaDate},
    {"level", initializeLevel, printLevel},
    {"sunrise", initializeSunrise, printSunrise},
    {"sundown", initializeSundown, printSundown},
    {"weather", initializeWeather, printWeather},
};

int printKey(const char *key, Record *record) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return keyMappings[i].print(record);
        }
    }
    return 0;
}

static int processKey(char *key, char *value, Record *record, int seen[7]) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            if (seen[i]) return 0;
            seen[i] = 1;
            return keyMappings[i].func(record, value, key);
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
