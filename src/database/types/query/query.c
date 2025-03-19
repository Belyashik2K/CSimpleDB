#include <stddef.h>
#include "query.h"
#include "order/sort.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "../../../utils/mem_profiler/helper.h"

void freeCondition(Condition *condition) {
    if (!condition) {
        return;
    }
    freeWrapper(condition->field);
    freeWrapper(condition->comparison);
    freeWrapper(condition->value);
    freeWrapper(condition);
}

void freeConditions(Condition **conditions, int conditionCount) {
    if (!conditions || !*conditions) {
        return;
    }
    for (int i = 0; i < conditionCount; i++) {
        Condition *condition = conditions[i];
        freeCondition(condition);
    }
    freeWrapper(conditions);
}

void freeField(QueryField *field) {
    if (!field) {
        return;
    }
    freeWrapper(field->field);
    freeWrapper(field->value);
    freeWrapper(field);
}

void freeFields(QueryField **fields, int fieldCount) {
    if (!fields) {
        return;
    }
    for (int i = 0; i < fieldCount; i++) {
        QueryField *field = fields[i];
        freeField(field);
    }
    freeWrapper(fields);
}

void freeQuery(Query *query) {
    if (!query) {
        return;
    }
    freeFields(query->fields, query->field_count);
    freeConditions(query->conditions, query->condition_count);
    freeWrapper(query);
}

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

QueryField *queryFieldFactory(char *fieldString) {
    QueryField *field = (QueryField *) mallocWrapper(sizeof(QueryField));
    field->value = NULL;

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
        field->field = strdupWrapper(trimWhitespace(fieldString));
        return field;
    }

    const int fieldLen = equalSign - fieldString;
    char *fieldPart = (char *) mallocWrapper(fieldLen + 1);
    if (!fieldPart) {
        freeField(field);
        return NULL;
    }
    strncpy(fieldPart, fieldString, fieldLen);
    fieldPart[fieldLen] = '\0';


    int valueLen = strlen(equalSign + 1);
    char *valuePart = (char *) mallocWrapper(valueLen + 1);
    if (!valuePart) {
        freeField(field);
        freeWrapper(fieldPart);
        return NULL;
    }
    strcpy(valuePart, equalSign + 1);

    field->field = strdupWrapper(trimWhitespace(fieldPart));
    field->value = strdupWrapper(trimWhitespace(valuePart));

    freeWrapper(fieldPart);
    freeWrapper(valuePart);
    return field;
}

Condition *conditionFactory(char *conditionString) {
    Condition *condition = (Condition *) mallocWrapper(sizeof(Condition));
    if (!condition) {
        return NULL;
    }

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
        freeCondition(condition);
        return NULL;
    }

    const int fieldLen = opPos - conditionString;
    char *fieldPart = (char *) mallocWrapper(fieldLen + 1);
    if (!fieldPart) {
        freeCondition(condition);
        return NULL;
    }
    strncpy(fieldPart, conditionString, fieldLen);
    fieldPart[fieldLen] = '\0';

    const int opLen = strlen(selectedOp);
    char *valuePart = strdupWrapper(opPos + opLen);
    if (!valuePart) {
        freeWrapper(fieldPart);
        freeCondition(condition);
        return NULL;
    }

    const char *trimmedField = trimWhitespace(fieldPart);
    const char *trimmedValue = trimWhitespace(valuePart);

    condition->field = strdupWrapper(trimmedField);
    condition->comparison = comparisonFactory(selectedOp);
    condition->value = strdupWrapper(trimmedValue);

    freeWrapper(fieldPart);
    freeWrapper(valuePart);
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

    char *action = (char *) mallocWrapper(endIndex + 1);
    strncpy(action, *line - endIndex, endIndex);
    action[endIndex] = '\0';
    return action;
}

int findFields(char **line, Query *query) {
    QueryField **fields = (QueryField **) mallocWrapper(sizeof(QueryField *));
    *line = skipSpaces(*line);

    int inQuotes = 0;
    char lastSeenQuoteCode = 0;
    int lastIndexOfField = 0;
    int fieldCount = 0;
    const unsigned long long strLength = strlen(*line);

    if (strLength == 0) {
        query->field_count = 0;
        query->fields = NULL;
        freeFields(fields, fieldCount);
    }

    const char *startPtr = *line;
    for (int i = 0; i <= strLength; i++) {
        checkQuotes(*line, i, &inQuotes, &lastSeenQuoteCode);

        if (!inQuotes && (*line)[i] == ',') {
            const long long endIndex = *line + i - startPtr;
            char *field = (char *) mallocWrapper(endIndex + 1);
            strncpy(field, startPtr, endIndex);
            field[endIndex] = '\0';

            QueryField *fieldObj = queryFieldFactory(prepareString(field));
            if (!fieldObj) {
                freeWrapper(field);
                freeFields(fields, fieldCount);
                return 0;
            }

            fields = (QueryField **) reallocWrapper(fields, sizeof(QueryField *) * (fieldCount + 1));
            fields[fieldCount++] = fieldObj;

            if (query->action.value == SORT) {
                if (!sortFactory(fields[fieldCount - 1]->value)) {
                    freeWrapper(field);
                    freeFields(fields, fieldCount);
                    return 0;
                }
            }

            freeWrapper(field);
            startPtr = *line + i + 1;
        }

        if (i == strLength || (*line)[i] == ' ' && !inQuotes) {
            const long long endIndex = *line + i - startPtr;
            char *field = (char *) mallocWrapper(endIndex + 1);
            strncpy(field, startPtr, endIndex);
            field[endIndex] = '\0';

            QueryField *fieldObj = queryFieldFactory(prepareString(field));
            if (!fieldObj) {
                freeWrapper(field);
                freeFields(fields, fieldCount);
                return 0;
            }

            fields = (QueryField **) reallocWrapper(fields, sizeof(QueryField *) * (fieldCount + 1));
            fields[fieldCount++] = fieldObj;

            if (query->action.value == SORT) {
                if (!sortFactory(fields[fieldCount - 1]->value)) {
                    freeWrapper(field);
                    freeFields(fields, fieldCount);
                    return 0;
                }
            }

            lastIndexOfField = i;
            freeWrapper(field);
            break;
        }

        if ((*line)[i] == '\0') {
            break;
        }
    }
    *line += lastIndexOfField;

    if (fieldCount == 0) {
        freeFields(fields, fieldCount);
        return 0;
    }

    query->fields = fields;
    query->field_count = fieldCount;
    return 1;
}

int findConditions(char **line, Query *query) {
    Condition **conditions = (Condition **) mallocWrapper(sizeof(Condition *));
    *line = skipSpaces(*line);

    int inQuotes = 0;
    char lastSeenQuoteCode = 0;
    int conditionsCount = 0;
    const unsigned long long strLength = strlen(*line);

    if (strLength == 0) {
        query->condition_count = 0;
        query->conditions = NULL;
        freeConditions(conditions, conditionsCount);
        return 0;
    }

    char *startPtr = *line;
    for (int i = 0; i <= strLength; i++) {
        checkQuotes(*line, i, &inQuotes, &lastSeenQuoteCode);

        if (!inQuotes && (*line)[i] == ' ' || i == strLength) {
            const long long endIndex = *line + i - startPtr;
            char *field = (char *) mallocWrapper(endIndex + 1);
            strncpy(field, startPtr, endIndex);
            field[endIndex] = '\0';

            if (!strcmp(skipSpaces(prepareString(field)), "") == 0) {
                Condition *condition = conditionFactory(prepareString(field));
                conditions = (Condition **) reallocWrapper(conditions, sizeof(Condition *) * (conditionsCount + 1));
                conditions[conditionsCount++] = condition;
            }

            freeWrapper(field);
            startPtr = *line + i;
        }

        if ((*line)[i] == '\0') {
            break;
        }
    }

    query->condition_count = conditionsCount;
    query->conditions = conditions;

    return 1;
}

Query *queryFactory(char *queryStr) {
    char *queryStrPtr = strdup(queryStr);
    char *queryStrCopy = queryStrPtr;
    if (!queryStrCopy)
        return NULL;


    char *actionStr = findAction(&queryStrCopy);
    const Action *action = actionFactory(actionStr);
    freeWrapper(actionStr);

    if (!action) {
        freeWrapper(queryStrPtr);
        return NULL;
    }

    Query *query = (Query *) mallocWrapper(sizeof(Query));
    if (!query) {
        return NULL;
    }
    query->action = *action;

    if (query->action.value != DELETE) {
        int isFieldsFound = findFields(&queryStrCopy, query);
        if (!isFieldsFound) {
            // free(query);
            // return NULL;
        }
    } else {
        query->fields = NULL;
        query->field_count = 0;
    }

    const int isConditionsFound = findConditions(&queryStrCopy, query);
    if (!isConditionsFound) {
        // Check for actions that require conditions
    }

    if (
        query->action.value == SORT && query->field_count == 0 ||
        query->action.value == SELECT && query->field_count == 0 ||
        query->action.value == INSERT && query->field_count == 0 ||
        query->action.value == DELETE && query->field_count != 0 ||
        query->action.value == UPDATE && query->field_count == 0 ||
        query->action.value == UNIQUE && query->field_count == 0 ||
        query->action.value == INSERT && query->condition_count != 0
    ) {
        freeWrapper(queryStrPtr);
        freeQuery(query);
        return NULL;
    }

    if (query->field_count != 0) {
        for (int i = 0; i < query->field_count; i++) {
            if (
                query->fields[i]->value && query->action.value == SELECT ||
                query->fields[i]->value && query->action.value == UNIQUE ||
                query->fields[i]->value && query->action.value == DELETE ||
                !query->fields[i]->value && query->action.value == UPDATE ||
                !query->fields[i]->value && query->action.value == INSERT ||
                !query->fields[i]->value && query->action.value == SORT
            ) {
                freeWrapper(queryStrPtr);
                freeQuery(query);
                return NULL;
            }
        }
    }

    if (query->condition_count != 0) {
        if (query->action.value == DELETE) {
            if (query->condition_count >= 1 && !query->conditions[0]->field && !query->conditions[0]->value) {
                freeQuery(query);
                return NULL;
            }
        }
    }

    freeWrapper(queryStrPtr);
    return query;
}
