#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../record/record.h"
#include "../query/query.h"

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
