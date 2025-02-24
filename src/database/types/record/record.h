#ifndef RECORD_H
#define RECORD_H

#include "../int/int.h"
#include "../string/string.h"
#include "../date/date.h"
#include "../time/time.h"
#include "../weather/weather.h"
#include "../query/query.h"

typedef struct {
    CustomInt geo_id;
    CustomString geo_pos;
    Date mea_date;
    CustomInt level;
    Time sunrise;
    Weather weather;
    Time sundown;
} Record;

typedef struct RecordNode {
    Record *data;
    struct RecordNode *next;
} RecordNode;


Record *recordFactory(Query *query);
int printKey(const char *key, Record *record);
int validateKey(char *key);
int validateValue(char *key, char *value);
int isSatisfiedByCondition(Record *record, Condition *condition);
int updateRecord(Record *record, QueryField *new_data);

#endif
