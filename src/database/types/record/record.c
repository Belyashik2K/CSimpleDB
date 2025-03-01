#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../record/record.h"
#include "../query/query.h"

static int initializeGeoId(Record *record, char *value, char *key) {
    CustomInt *tmp = intFactory(value, key);
    if (!tmp) return 0;
    record->geo_id = tmp;
    return 1;
}

static int initializeGeoPos(Record *record, char *value, char *key) {
    CustomString *tmp = stringFactory(value, key);
    if (!tmp) return 0;
    record->geo_pos = tmp;
    return 1;
}

static int initializeMeaDate(Record *record, char *value, char *key) {
    Date *tmp = dateFactory(value, key);
    if (!tmp) return 0;
    record->mea_date = tmp;
    return 1;
}

static int initializeLevel(Record *record, char *value, char *key) {
    CustomInt *tmp = intFactory(value, key);
    if (!tmp) return 0;
    record->level = tmp;
    return 1;
}

static int initializeSunrise(Record *record, char *value, char *key) {
    Time *tmp = timeFactory(value, key);
    if (!tmp) return 0;
    record->sunrise = tmp;
    return 1;
}

static int initializeSundown(Record *record, char *value, char *key) {
    Time *tmp = timeFactory(value, key);
    if (!tmp) return 0;
    record->sundown = tmp;
    return 1;
}

static int initializeWeather(Record *record, char *value, char *key) {
    Weather *tmp = weatherFactory(value, key);
    if (!tmp) return 0;
    record->weather = tmp;
    return 1;
}

static int printGeoId(Record *record) {
    printf("%s", record->geo_id->toString(record->geo_id));
    return 1;
}

static int printGeoPos(Record *record) {
    printf("%s", record->geo_pos->toString(record->geo_pos));
    return 1;
}

static int printMeaDate(Record *record) {
    printf("%s", record->mea_date->toString(record->mea_date));
    return 1;
}

static int printLevel(Record *record) {
    printf("%s", record->level->toString(record->level));
    return 1;
}

static int printSunrise(Record *record) {
    printf("%s", record->sunrise->toString(record->sunrise));
    return 1;
}

static int printSundown(Record *record) {
    printf("%s", record->sundown->toString(record->sundown));
    return 1;
}

static int printWeather(Record *record) {
    printf("%s", record->weather->toString(record->weather));
    return 1;
}

int validateGeoId(char *value) {
    return intFactory(value, "geo_id") != NULL;
}

int validateGeoPos(char *value) {
    return stringFactory(value, "geo_pos") != NULL;
}

int validateMeaDate(char *value) {
    return dateFactory(value, "mea_date") != NULL;
}

int validateLevel(char *value) {
    return intFactory(value, "level") != NULL;
}

int validateSunrise(char *value) {
    return timeFactory(value, "sunrise") != NULL;
}

int validateSundown(char *value) {
    return timeFactory(value, "sundown") != NULL;
}

int validateWeather(char *value) {
    return weatherFactory(value, "weather") != NULL;
}

typedef int (*init_func)(Record *, char *, char *);

typedef int (*print_func)(Record *);

typedef int (*validate_func)(char *);

typedef int (*compare_func)(Record *, char *, ComparisonOptionEnum);

typedef int (*update_func)(Record *, char *);

typedef char *(*toString_func)(void *);

typedef int (*compareTwo_func)(Record *, Record *, ComparisonOptionEnum);

typedef struct {
    const char *key;
    init_func initialize;
    print_func print;
    validate_func validate;
    compare_func compare;
    update_func update;
    toString_func toString;
    compareTwo_func compareTwo;
} KeyMap;

char *jumpToEqualSign(char *str) {
    while (*str != '=' && *str != '\0') {
        str++;
    }
    str++;
    return str;
}

int compareGeoId(Record *record, char *other, ComparisonOptionEnum option) {
    return record->geo_id->compare(record->geo_id, other, option);
}

int compareGeoPos(Record *record, char *other, ComparisonOptionEnum option) {
    return record->geo_pos->compare(record->geo_pos, other, option);
}

int compareMeaDate(Record *record, char *other, ComparisonOptionEnum option) {
    return record->mea_date->compare(record->mea_date, other, option);
}

int compareLevel(Record *record, char *other, ComparisonOptionEnum option) {
    return record->level->compare(record->level, other, option);
}

int compareSunrise(Record *record, char *other, ComparisonOptionEnum option) {
    return record->sunrise->compare(record->sunrise, other, option);
}

int compareSundown(Record *record, char *other, ComparisonOptionEnum option) {
    return record->sundown->compare(record->sundown, other, option);
}

int compareWeather(Record *record, char *other, ComparisonOptionEnum option) {
    return record->weather->compare(record->weather, other, option);
}

int updateGeoId(Record *record, char *newValue) {
    return record->geo_id->update(record->geo_id, newValue);
}

int updateGeoPos(Record *record, char *newValue) {
    return record->geo_pos->update(record->geo_pos, newValue);
}

int updateMeaDate(Record *record, char *newValue) {
    return record->mea_date->update(record->mea_date, newValue);
}

int updateLevel(Record *record, char *newValue) {
    return record->level->update(record->level, newValue);
}

int updateSunrise(Record *record, char *newValue) {
    return record->sunrise->update(record->sunrise, newValue);
}

int updateSundown(Record *record, char *newValue) {
    return record->sundown->update(record->sundown, newValue);
}

int updateWeather(Record *record, char *newValue) {
    return record->weather->update(record->weather, newValue);
}

char *getGeoIdStringRepresentation(Record *record) {
    return record->geo_id->toString(record->geo_id);
}

char *getGeoPosStringRepresentation(Record *record) {
    return record->geo_pos->toString(record->geo_pos);
}

char *getMeaDateStringRepresentation(Record *record) {
    return record->mea_date->toString(record->mea_date);
}

char *getLevelStringRepresentation(Record *record) {
    return record->level->toString(record->level);
}

char *getSunriseStringRepresentation(Record *record) {
    return record->sunrise->toString(record->sunrise);
}

char *getSundownStringRepresentation(Record *record) {
    return record->sundown->toString(record->sundown);
}

char *getWeatherStringRepresentation(Record *record) {
    return record->weather->toString(record->weather);
}

int compareTwoGeoId(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->geo_id->compare(record->geo_id, jumpToEqualSign(getGeoIdStringRepresentation(other)), option);
}

int compareTwoGeoPos(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->geo_pos->compare(record->geo_pos, jumpToEqualSign(getGeoPosStringRepresentation(other)), option);
}

int compareTwoMeaDate(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->mea_date->compare(record->mea_date, jumpToEqualSign(getMeaDateStringRepresentation(other)), option);
}

int compareTwoLevel(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->level->compare(record->level, jumpToEqualSign(getLevelStringRepresentation(other)), option);
}

int compareTwoSunrise(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->sunrise->compare(record->sunrise, jumpToEqualSign(getSunriseStringRepresentation(other)), option);
}

int compareTwoSundown(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->sundown->compare(record->sundown, jumpToEqualSign(getSundownStringRepresentation(other)), option);
}

int compareTwoWeather(Record *record, Record *other, ComparisonOptionEnum option) {
    return record->weather->compare(record->weather, jumpToEqualSign(getWeatherStringRepresentation(other)), option);
}

static KeyMap keyMappings[] = {
    {
        "geo_id", initializeGeoId, printGeoId, validateGeoId, compareGeoId, updateGeoId, getGeoIdStringRepresentation,
        compareTwoGeoId
    },
    {
        "geo_pos", initializeGeoPos, printGeoPos, validateGeoPos, compareGeoPos, updateGeoPos,
        getGeoPosStringRepresentation, compareTwoGeoPos
    },
    {
        "mea_date", initializeMeaDate, printMeaDate, validateMeaDate, compareMeaDate, updateMeaDate,
        getMeaDateStringRepresentation, compareTwoMeaDate
    },
    {
        "level", initializeLevel, printLevel, validateLevel, compareLevel, updateLevel, getLevelStringRepresentation,
        compareTwoLevel
    },
    {
        "sunrise", initializeSunrise, printSunrise, validateSunrise, compareSunrise, updateSunrise,
        getSunriseStringRepresentation, compareTwoSunrise
    },
    {
        "sundown", initializeSundown, printSundown, validateSundown, compareSundown, updateSundown,
        getSundownStringRepresentation, compareTwoSundown
    },
    {
        "weather", initializeWeather, printWeather, validateWeather, compareWeather, updateWeather,
        getWeatherStringRepresentation, compareTwoWeather
    }
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

int compareTwoRecords(Record *record, Record *other, ComparisonOptionEnum option, QueryField *field) {
    for (int i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
        if (strcmp(field->field, keyMappings[i].key) == 0) {
            if (keyMappings[i].compareTwo(record, other, EQUAL)) {
                return 0;
            }
            if (keyMappings[i].compareTwo(record, other, LESS)) {
                if (option == LESS) return 1;
                return -1;
            }
            if (keyMappings[i].compareTwo(record, other, GREATER)) {
                if (option == GREATER) return 1;
                return -1;
            }
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
