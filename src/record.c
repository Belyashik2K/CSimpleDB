#include "../include/record.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int skipInsert(const char *recordString) {
    return recordString && strncmp(recordString, "insert ", 7) == 0;
}

static int initializeGeoId(Record *record, char *value) {
    CustomInt *geoID = intFactory(value);
    if (!geoID) return 0;
    record->geo_id = *geoID;
    return 1;
}

static int initializeGeoPos(Record *record, char *value) {
    CustomString *geoPos = stringFactory(value);
    if (!geoPos) return 0;
    record->geo_pos = *geoPos;
    return 1;
}

static int initializeMeaDate(Record *record, char *value) {
    Date *meaDate = dateFactory(value);
    if (!meaDate) return 0;
    record->mea_date = *meaDate;
    return 1;
}

static int initializeLevel(Record *record, char *value) {
    CustomInt *level = intFactory(value);
    if (!level) return 0;
    record->level = *level;
    return 1;
}

static int initializeSunrise(Record *record, char *value) {
    Time *sunrise = timeFactory(value);
    if (!sunrise) return 0;
    record->sunrise = *sunrise;
    return 1;
}

static int initializeSundown(Record *record, char *value) {
    Time *sundown = timeFactory(value);
    if (!sundown) return 0;
    record->sundown = *sundown;
    return 1;
}

static int initializeWeather(Record *record, char *value) {
    record->weather = weather_from_string(value);
    if (record->weather == WEATHER_COUNT) return 0;
    return 1;
}

typedef int (*init_func)(Record *, char *);

typedef struct {
    const char *key;
    init_func func;
} KeyMap;

static KeyMap keyMappings[] = {
    {"geo_id", initializeGeoId},
    {"geo_pos", initializeGeoPos},
    {"mea_date", initializeMeaDate},
    {"level", initializeLevel},
    {"sunrise", initializeSunrise},
    {"sundown", initializeSundown},
    {"weather", initializeWeather},
};

static int processKey(const char *key, char *value, Record *record) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(key, keyMappings[i].key) == 0) {
            return keyMappings[i].func(record, value);
        }
    }
    return 0;
}

Record *recordFactory(char *recordString) {
    if (!skipInsert(recordString))
        return NULL;

    recordString += 7;

    Record *record = malloc(sizeof(Record));
    if (!record)
        return NULL;

    char *recordCopy = strdup(recordString);
    if (!recordCopy) return NULL;
    const char *token = strtok(recordCopy, ",");
    while (token) {
        char key[20] = {0}, value[50] = {0};

        if (sscanf(token, " %19[^=]=%49[^\n]", key, value) == 2) {
            if (!processKey(key, value, record)) {
                free(record);
                return NULL;
            }
        }
        token = strtok(NULL, ",");
    }
    return record;
}
