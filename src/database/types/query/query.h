#ifndef QUERY_H
#define QUERY_H

#include "condition/condition.h"
#include "action/action.h"

typedef struct {
    char *field;
    char *value;
} QueryField;

typedef struct {
    Action action;
    QueryField **fields;
    int field_count;
    Condition **conditions;
    int condition_count;
} Query;


Query *queryFactory(char *queryStr);
char *trimWhitespace(char *str);
void freeQuery(Query *query);

#endif
