#include <stddef.h>
#include "query.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* trimWhitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == '\0')
        return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end+1) = '\0';
    return str;
}

QueryField queryFieldFactory(char *fieldString) {
    char *queryFieldPtr = NULL;
    const char *field = strtok_r(fieldString, "=", &queryFieldPtr);
    const char *value = strtok_r(NULL, "=", &queryFieldPtr);

    QueryField queryField;
    queryField.field = strdup(field);
    queryField.value = strdup(value);

    return queryField;
}

Condition conditionFactory(char *conditionString) {
    Condition condition;
    condition.field = NULL;
    condition.comparison = NULL;
    condition.value = NULL;

    char *opPos = NULL;
    const char *selectedOp = NULL;

    for (int i = 0; i < COMPARE_COUNT; i++) {
        opPos = strstr(conditionString, comparisonOptionStrings[i]);
        if (opPos != NULL) {
            selectedOp = comparisonOptionStrings[i];
            break;
        }
    }
    if (selectedOp == NULL) {
        return condition;
    }

    const int fieldLen = opPos - conditionString;
    char *fieldPart = malloc(fieldLen + 1);
    if (!fieldPart)
        return condition;
    strncpy(fieldPart, conditionString, fieldLen);
    fieldPart[fieldLen] = '\0';

    const int opLen = strlen(selectedOp);
    char *valuePart = strdup(opPos + opLen);
    if (!valuePart) {
        free(fieldPart);
        return condition;
    }

    const char *trimmedField = trimWhitespace(fieldPart);
    const char *trimmedValue = trimWhitespace(valuePart);

    condition.field = strdup(trimmedField);
    condition.comparison = comparisonFactory(selectedOp);
    condition.value = strdup(trimmedValue);

    free(fieldPart);
    free(valuePart);
    return condition;
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
    char *field = strtok_r(fields, ",", &querySavePtr);
    int fieldCount = 0;
    query->fields = malloc(maxTokens * sizeof(QueryField));
    while (field != NULL) {
        query->fields[fieldCount++] = queryFieldFactory(field);
        field = strtok_r(NULL, ",", &querySavePtr);
    }

    query->conditions = malloc(maxTokens * sizeof(Condition));
    int conditionCount = 0;
    if (tokenCount > 2) {
        for (int i = 2; i < tokenCount; i++) {
            const Condition condition = conditionFactory(tokens[i]);
            query->conditions[conditionCount++] = condition;
        }
    }

    query->action = *action;
    query->field_count = fieldCount;
    query->condition_count = conditionCount;

    free(tokens);
    free(queryStrCopy);

    return query;
}
