#include <stddef.h>
#include "query.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

QueryField *queryFieldFactory(const char *fieldString) {
    char *fieldStringCopy = strdup(fieldString);
    if (!fieldStringCopy)
        return NULL;

    char *queryFieldPtr = NULL;
    const char *field = strtok_r(fieldStringCopy, "=", &queryFieldPtr);
    const char *value = strtok_r(NULL, "=", &queryFieldPtr);

    QueryField *queryField = malloc(sizeof(QueryField));
    if (!queryField) {
        free(fieldStringCopy);
        return NULL;
    }

    queryField->field = strdup(field);
    queryField->value = strdup(value);

    free(fieldStringCopy);
    return queryField;
}


Query *queryFactory(char *queryStr) {
    char *queryStrCopy = strdup(queryStr);
    if (!queryStrCopy)
        return NULL;

    const int maxTokens = 100;
    char **tokens = malloc(maxTokens * sizeof(char *));
    if (!tokens) {
        free(queryStrCopy);
        return NULL;
    }

    int tokenCount = 0;
    char *token = strtok(queryStrCopy, " \t\n");
    char *querySavePtr = NULL;

    const Action *action = actionFactory(token);
    if (!action) {
        free(tokens);
        free(queryStrCopy);
        return NULL;
    }

    while (token && tokenCount < maxTokens) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, " \t\n");
    }

    Query *query = malloc(sizeof(Query));
    if (!query) {
        free(tokens);
        free(queryStrCopy);
        return NULL;
    }

    char *fields = strdup(tokens[1]);
    const char *field = strtok_r(fields, ",", &querySavePtr);
    int fieldCount = 0;
    query->fields = malloc(maxTokens * sizeof(QueryField));
    while (field != NULL) {
        query->fields[fieldCount++] = *queryFieldFactory(field);
        field = strtok_r(NULL, ",", &querySavePtr);
    }

    for (int i = 0; i < fieldCount; i++) {
        printf("Field: %s, Value: %s\n", query->fields[i].field, query->fields[i].value);
    }

    query->action = *action;
    query->field_count = fieldCount;

    free(tokens);
    free(queryStrCopy);

    return query;
}
