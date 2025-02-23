#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../record/record.h"

static int skipInsert(const char *recordString) {
    return recordString && strncmp(recordString, "insert ", 7) == 0;
}

static int initializeGeoId(Record *record, char *value, char *key) {
    CustomInt *geoID = intFactory(value, key);
    if (!geoID) return 0;
    record->geo_id = *geoID;
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
    char token_counter = 0;
    int seen[7] = {0};
    while (token) {
        char key[20] = {0}, value[50] = {0};

        if (sscanf(token, " %19[^=]=%49[^\n]", key, value) == 2) {
            if (!processKey(key, value, record, seen)) {
                free(record);
                return NULL;
            }
        }
        token = strtok(NULL, ",");
        token_counter++;
    }

    if (token_counter != 7) {
        free(record);
        return NULL;
    }

    return record;
}
