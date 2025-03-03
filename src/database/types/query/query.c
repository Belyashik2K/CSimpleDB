#include <stddef.h>
#include "query.h"
#include "order/sort.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int checkForScreeningInQuery(const char *line, const int index) {
    return index > 0 && line[index - 1] == '\\' && line[index - 2] != '\\';
}

void checkBrackets(const char *line, const int index, int *inBrackets, char *lastSeenBracketCode) {
    if (line[index] == '[' || line[index] == ']') {
        if (*inBrackets == 0) {
            *inBrackets = 1;
            *lastSeenBracketCode = line[index];
        } else if (*lastSeenBracketCode != line[index]) {
            *inBrackets = 0;
            *lastSeenBracketCode = 0;
        }
    }
}

void checkQuotes(const char *line, const int index, int *inQuotes, char *lastSeenQuoteCode) {
    if (line[index] == '"' || line[index] == '\'') {
        if (checkForScreeningInQuery(line, index)) {
            return;
        }

        if (*inQuotes == 0) {
            *inQuotes = 1;
            *lastSeenQuoteCode = line[index];
        } else if (*lastSeenQuoteCode == line[index]) {
            *inQuotes = 0;
            *lastSeenQuoteCode = 0;
        }
    }
}

char *prepareString(char *str) {
    str[strcspn(str, "\n")] = 0;
    str = trimWhitespace(str);
    return str;
}

char *trimWhitespace(char *str) {
    while (isspace((unsigned char) *str)) str++;
    if (*str == '\0')
        return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
        end--;
    *(end + 1) = '\0';
    return str;
}

QueryField queryFieldFactory(char *fieldString) {
    QueryField queryField = {NULL, NULL};
    int inQuotes = 0;
    char quoteChar = 0;

    char *pos = fieldString;
    char *equalSign = NULL;

    while (*pos) {
        if (*pos == '"' || *pos == '\'') {
            if (!checkForScreeningInQuery(pos, pos - fieldString)) {
                if (!inQuotes) {
                    inQuotes = 1;
                    quoteChar = *pos;
                } else if (*pos == quoteChar) {
                    inQuotes = 0;
                }
            }
        } else if (*pos == '=' && !inQuotes && !equalSign) {
            equalSign = pos;
        }
        pos++;
    }

    if (!equalSign) {
        queryField.field = strdup(trimWhitespace(fieldString));
        return queryField;
    }

    int fieldLen = equalSign - fieldString;
    char *fieldPart = malloc(fieldLen + 1);
    if (!fieldPart) {
        return queryField;
    }
    strncpy(fieldPart, fieldString, fieldLen);
    fieldPart[fieldLen] = '\0';

    int valueLen = strlen(equalSign + 1);
    char *valuePart = malloc(valueLen + 1);
    if (!valuePart) {
        free(fieldPart);
        return queryField;
    }
    strcpy(valuePart, equalSign + 1);

    queryField.field = strdup(trimWhitespace(fieldPart));
    queryField.value = strdup(trimWhitespace(valuePart));

    free(fieldPart);
    free(valuePart);
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

char *skipSpaces(char *line) {
    while (*line == ' ') {
        line++;
    }
    return line;
}

char *findAction(char **line) {
    *line = skipSpaces(*line);

    int endIndex = 0;
    while (**line != ' ') {
        if (**line == '\0') {
            printf("Error: No action found\n");
            break;
        }
        endIndex++;
        (*line)++;
    }

    char *action = malloc(endIndex + 1);
    strncpy(action, *line - endIndex, endIndex);
    action[endIndex] = '\0';
    return action;
}

QueryField *findFields(char **line, Query *query) {
    QueryField *fields = malloc(sizeof(QueryField));
    *line = skipSpaces(*line);

    int inQuotes = 0;
    char lastSeenQuoteCode = 0;
    int lastIndexOfField = 0;
    int fieldCount = 0;
    const unsigned long long strLength = strlen(*line);

    if (strLength == 0) {
        query->field_count = 0;
        query->fields = fields;
        return fields;
    }

    const char *startPtr = *line;
    for (int i = 0; i <= strLength; i++) {
        checkQuotes(*line, i, &inQuotes, &lastSeenQuoteCode);

        if (!inQuotes && (*line)[i] == ',') {
            const long long endIndex = *line + i - startPtr;
            char *field = malloc(endIndex + 1);
            strncpy(field, startPtr, endIndex);
            field[endIndex] = '\0';

            const QueryField fieldObj = queryFieldFactory(prepareString(field));
            fields = realloc(fields, sizeof(QueryField) * (fieldCount + 1));
            fields[fieldCount++] = fieldObj;

            if (query->action.value == SORT) {
                if (!sortFactory(fields[fieldCount - 1].value)) {
                    free(fields);
                    return NULL;
                }
            }

            free(field);
            startPtr = *line + i + 1;
        }

        if (i == strLength || (*line)[i] == ' ' && !inQuotes) {
            const long long endIndex = *line + i - startPtr;
            char *field = malloc(endIndex + 1);
            strncpy(field, startPtr, endIndex);
            field[endIndex] = '\0';

            const QueryField fieldObj = queryFieldFactory(prepareString(field));
            fields = realloc(fields, sizeof(QueryField) * (fieldCount + 1));
            fields[fieldCount++] = fieldObj;

            if (query->action.value == SORT) {
                if (!sortFactory(fields[fieldCount - 1].value)) {
                    free(fields);
                    return NULL;
                }
            }

            lastIndexOfField = i;
            free(field);
            break;
        }

        if ((*line)[i] == '\0') {
            break;
        }
    }
    *line += lastIndexOfField;

    if (fieldCount == 0) {
        free(fields);
        return NULL;
    }

    query->fields = fields;
    query->field_count = fieldCount;
    return fields;
}

Condition *findConditions(char **line, Query *query) {
    Condition *conditions = malloc(sizeof(Condition));
    *line = skipSpaces(*line);

    int inQuotes = 0;
    char lastSeenQuoteCode = 0;
    int conditionsCount = 0;
    const unsigned long long strLength = strlen(*line);

    if (strLength == 0) {
        query->condition_count = 0;
        query->conditions = conditions;
        return conditions;
    }

    char *startPtr = *line;
    for (int i = 0; i <= strLength; i++) {
        checkQuotes(*line, i, &inQuotes, &lastSeenQuoteCode);

        if (!inQuotes && (*line)[i] == ' ' || i == strLength) {
            const long long endIndex = *line + i - startPtr;
            char *field = malloc(endIndex + 1);
            strncpy(field, startPtr, endIndex);
            field[endIndex] = '\0';

            if (!strcmp(skipSpaces(prepareString(field)), "") == 0) {
                const Condition condition = conditionFactory(prepareString(field));
                conditions = realloc(conditions, sizeof(Condition) * (conditionsCount + 1));
                conditions[conditionsCount++] = condition;
            }

            startPtr = *line + i;
        }

        if ((*line)[i] == '\0') {
            break;
        }
    }

    query->condition_count = conditionsCount;
    query->conditions = conditions;

    return conditions;
}

// int validateFields(QueryField *field, Query *query) {
//     if (query->action.value == SELECT && field->value) {
//         return 0;
//     }
//
//     if (query->action.value == INSERT && !field->value) {
//         return 0;
//     }
//
//     if (query->action.value == DELETE && field->field) {
//         return 0;
//     }
//
//     if (query->action.value == UPDATE && !field->value) {
//         return 0;
//     }
//
//     return 1;
// }


Query *queryFactory(char *queryStr) {
    char *queryStrCopy = strdup(queryStr);
    if (!queryStrCopy)
        return NULL;


    char *actionStr = findAction(&queryStrCopy);
    const Action *action = actionFactory(actionStr);
    if (!action) {
        free(actionStr);
        // free(queryStrCopy);
        return NULL;
    }

    Query *query = malloc(sizeof(Query));

    if (!query) {
        free(queryStrCopy);
        return NULL;
    }
    query->action = *action;

    if (query->action.value != DELETE) {
        QueryField *fields = findFields(&queryStrCopy, query);
        if (!fields) {
            free(queryStrCopy);
            free(query);
            return NULL;
        }
    } else {
        query->field_count = 0;
    }

    Condition *conditions = findConditions(&queryStrCopy, query);
    if (!conditions) {
        free(queryStrCopy);
        free(query);
        return NULL;
    }

    if (
        query->action.value == SORT && query->field_count == 0 ||
        query->action.value == SELECT && query->field_count == 0 ||
        query->action.value == INSERT && query->field_count == 0 ||
        query->action.value == DELETE && query->field_count != 0 ||
        query->action.value == UPDATE && query->field_count == 0 ||
        query->action.value == UNIQUE && query->field_count == 0
    ) {
        free(query);
        return NULL;
    }

    return query;
}
