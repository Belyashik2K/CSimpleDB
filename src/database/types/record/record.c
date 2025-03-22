#include <stddef.h>
#include <string.h>

#include "../record/record.h"

#include <stdio.h>

#include "../query/query.h"
#include "../../../utils/mem_profiler/helper.h"

void freeRecordNode(RecordNode *node) {
    if (!node) return;

    freeRecord(node->data);
    freeWrapper(node);
}

void freeRecord(Record *record) {
    if (!record) return;

    if (record->geo_id) freeInt(record->geo_id);
    if (record->geo_pos) freeString(record->geo_pos);
    if (record->mea_date) freeDate(record->mea_date);
    if (record->level) freeInt(record->level);
    if (record->sunrise) freeTime(record->sunrise);
    if (record->sundown) freeTime(record->sundown);
    if (record->weather) freeWeather(record->weather);
    freeWrapper(record);
}


char *jumpToEqualSign(char *str) {
    while (*str != '=' && *str != '\0') {
        str++;
    }
    str++;
    return str;
}

#define INIT_FIELD(name, field, type, factory) \
    static int initialize##name(Record *record, char *value, char *key) { \
    record->field = factory(value, key); \
    return record->field != NULL; \
}

#define VALIDATE_FIELD(name, type, factory, freeFunc) \
    int validate##name(char *value) { \
        type* temp = factory(value, #name); \
        const int result = temp != NULL; \
        if (temp) freeFunc(temp); \
        return result; \
    }

#define COMPARE_FIELD(name, field) \
    int compare##name(Record *record, char *other, ComparisonOptionEnum option) { \
        return record->field->compare(record->field, other, option); \
    }

#define UPDATE_FIELD(name, field) \
    int update##name(Record *record, char *newValue) { \
        return record->field->update(record->field, newValue); \
    }

#define GET_STRING_REPRESENTATION(name, field) \
    char *get##name##StringRepresentation(Record *record) { \
        return record->field->toString(record->field); \
    }

#define COMPARE_TWO_RECORDS(name, field) \
    int compareTwo##name(Record *record, Record *other, ComparisonOptionEnum option) { \
        char *otherVal = get##name##StringRepresentation(other); \
        if (!otherVal) return 0; \
        int result = record->field->compare(record->field, otherVal, option); \
        freeWrapper(otherVal); \
        return result; \
    }

#define REGISTER_FIELD(name, field, type, factory, freeFunc) \
    INIT_FIELD(name, field, type, factory) \
    VALIDATE_FIELD(name, type, factory, freeFunc) \
    COMPARE_FIELD(name, field) \
    UPDATE_FIELD(name, field) \
    GET_STRING_REPRESENTATION(name, field) \
    COMPARE_TWO_RECORDS(name, field)


REGISTER_FIELD(GeoId, geo_id, CustomInt, intFactory, freeInt)

REGISTER_FIELD(GeoPos, geo_pos, CustomString, stringFactory, freeString)

REGISTER_FIELD(MeaDate, mea_date, Date, dateFactory, freeDate)

REGISTER_FIELD(Level, level, CustomInt, intFactory, freeInt)

REGISTER_FIELD(Sunrise, sunrise, Time, timeFactory, freeTime)

REGISTER_FIELD(Sundown, sundown, Time, timeFactory, freeTime)

REGISTER_FIELD(Weather, weather, Weather, weatherFactory, freeWeather)


typedef struct {
    const char *key;

    int (*initialize)(Record *, char *, char *);

    int (*validate)(char *);

    int (*compare)(Record *, char *, ComparisonOptionEnum);

    int (*update)(Record *, char *);

    char *(*toString)(Record *);

    int (*compareTwo)(Record *, Record *, ComparisonOptionEnum);
} KeyMap;

static KeyMap keyMappings[] = {
    {
        "geo_id",
        initializeGeoId,
        validateGeoId,
        compareGeoId,
        updateGeoId,
        getGeoIdStringRepresentation,
        compareTwoGeoId
    },
    {
        "geo_pos",
        initializeGeoPos,
        validateGeoPos,
        compareGeoPos,
        updateGeoPos,
        getGeoPosStringRepresentation,
        compareTwoGeoPos
    },
    {
        "mea_date",
        initializeMeaDate,
        validateMeaDate,
        compareMeaDate,
        updateMeaDate,
        getMeaDateStringRepresentation,
        compareTwoMeaDate
    },
    {
        "level",
        initializeLevel,
        validateLevel,
        compareLevel,
        updateLevel,
        getLevelStringRepresentation,
        compareTwoLevel
    },
    {
        "sunrise",
        initializeSunrise,
        validateSunrise,
        compareSunrise,
        updateSunrise,
        getSunriseStringRepresentation,
        compareTwoSunrise
    },
    {
        "sundown",
        initializeSundown,
        validateSundown,
        compareSundown,
        updateSundown,
        getSundownStringRepresentation,
        compareTwoSundown
    },
    {
        "weather",
        initializeWeather,
        validateWeather,
        compareWeather,
        updateWeather,
        getWeatherStringRepresentation,
        compareTwoWeather
    }
};

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
            return keyMappings[i].compare(record, condition->value, condition->comparison->cOperator);
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
    Record *record = (Record *) mallocWrapper(sizeof(Record));
    if (!record)
        return NULL;

    int seen[7] = {0};
    for (int i = 0; i < query->field_count; i++) {
        QueryField *field = query->fields[i];
        if (!processKey(field->field, field->value, record, seen)) {
            freeRecord(record);
            return NULL;
        }
    }

    for (int i = 0; i < 7; i++) {
        if (!seen[i]) {
            freeRecord(record);
            return NULL;
        }
    }

    return record;
}
