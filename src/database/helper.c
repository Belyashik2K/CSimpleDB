#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "initializer.h"
#include "types/record/record.h"
#include "types/query/action/action.h"
#include "../utils/writer/file_writer.h"

int makeInsertQuery(Database *db, Query *query) {
    if (!db || !query) return 0;

    RecordNode *newNode = malloc(sizeof(RecordNode));
    if (!newNode) return 0;

    Record *record = recordFactory(query);
    if (!record) {
        free(newNode);
        return 0;
    }

    newNode->data = record;
    newNode->next = NULL;

    if (db->tail) {
        db->tail->next = newNode;
    } else {
        db->head = newNode;
    }
    db->tail = newNode;
    db->size++;

    writeCountOfAffectedRecordsToFile(INSERT, db->size);

    return 1;
}

int checkIfRecordSatisfiesConditions(Record *record, Query *query) {
    if (!record || !query) return 0;

    for (int i = 0; i < query->condition_count; i++) {
        if (!isSatisfiedByCondition(record, &query->conditions[i])) {
            return 0;
        }
    }

    return 1;
}

int countAffectedRecords(Database *db, Query *query) {
    if (!db || !query) return 0;

    const RecordNode *current = db->head;

    int count = 0;

    while (current) {
        if (checkIfRecordSatisfiesConditions(current->data, query)) {
            count++;
        }
        current = current->next;
    }

    return count;
}

int makeSelectQuery(Database *db, Query *query) {
    if (!db || !query || !db->head) return 0;

    const RecordNode *current = db->head;

    const int count = countAffectedRecords(db, query);
    writeCountOfAffectedRecordsToFile(SELECT, count);

    while (current) {
        if (checkIfRecordSatisfiesConditions(current->data, query)) {
            writeSelectResultToFile(current->data, query);
        }
        current = current->next;
    }
    return 1;
}

int makeDeleteQuery(Database *db, Query *query) {
    if (!db || !query) return 0;

    RecordNode *current = db->head;
    RecordNode *prev = NULL;

    const int count = countAffectedRecords(db, query);
    writeCountOfAffectedRecordsToFile(DELETE, count);

    while (current) {
        if (checkIfRecordSatisfiesConditions(current->data, query)) {
            RecordNode *toDelete = current;

            if (prev == NULL) {
                db->head = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;

            free(toDelete->data);
            free(toDelete);
            db->size--;
        } else {
            prev = current;
            current = current->next;
        }
    }

    return 1;
}


int makeUpdateQuery(Database *db, Query *query) {
    if (!db || !query) return 0;

    const RecordNode *current = db->head;
    int updatedRecords = 0;

    while (current) {
        if (!checkIfRecordSatisfiesConditions(current->data, query)) {
            current = current->next;
            continue;
        }

        for (int i = 0; i < query->field_count; i++) {
            QueryField field = query->fields[i];
            updateRecord(current->data, &field);
        }
        updatedRecords++;
        current = current->next;
    }

    writeCountOfAffectedRecordsToFile(UPDATE, updatedRecords);

    return 1;
}

int makeUniqQuery(Database *db, Query *query) {
    if (!db || !query || query->field_count == 0) return 0;

    RecordNode *current = db->head;
    RecordNode *prev = NULL;
    int deletedCount = 0;

    while (current) {
        RecordNode *runner = current->next;
        int isDuplicate = 0;

        while (runner) {
            int fieldsMatch = 1;

            for (int i = 0; i < query->field_count; i++) {
                const QueryField field = query->fields[i];
                const char *currentValue = getFieldStringRepresentation(field.field, current->data);
                const char *runnerValue = getFieldStringRepresentation(field.field, runner->data);

                if (strcmp(currentValue, runnerValue) != 0) {
                    fieldsMatch = 0;
                    break;
                }
            }

            if (fieldsMatch) {
                isDuplicate = 1;
                break;
            }
            runner = runner->next;
        }

        RecordNode *nextNode = current->next;

        if (isDuplicate) {
            if (prev) {
                prev->next = nextNode;
            } else {
                db->head = nextNode;
            }

            if (current == db->tail) {
                db->tail = prev;
            }

            free(current->data);
            free(current);
            db->size--;
            deletedCount++;
            current = nextNode;
        } else {
            prev = current;
            current = nextNode;
        }
    }

    writeCountOfAffectedRecordsToFile(UNIQUE, deletedCount);
    return 1;
}

RecordNode *merge(RecordNode *left, RecordNode *right, Query *query) {
    RecordNode dummy = {0};
    RecordNode *tail = &dummy;

    while (left && right) {
        int compareResult = 0;

        for (int i = 0; i < query->field_count && compareResult == 0; i++) {
            QueryField field = query->fields[i];
            ComparisonOptionEnum option = strcmp(query->fields[i].value, "asc") == 0 ? GREATER : LESS;
            compareResult = compareTwoRecords(left->data, right->data, option, &field);
        }

        if (compareResult <= 0) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    if (left) tail->next = left;
    if (right) tail->next = right;

    return dummy.next;
}

void splitList(RecordNode *head, RecordNode **left, RecordNode **right) {
    if (!head || !head->next) {
        *left = head;
        *right = NULL;
        return;
    }

    RecordNode *slow = head, *fast = head->next;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    *left = head;
    *right = slow->next;
    slow->next = NULL;
}

RecordNode *mergeSort(RecordNode *head, Query *query) {
    if (!head || !head->next) return head;

    RecordNode *left, *right;
    splitList(head, &left, &right);

    left = mergeSort(left, query);
    right = mergeSort(right, query);

    return merge(left, right, query);
}

int makeSortQuery(Database *db, Query *query) {
    if (!db || !query || query->field_count == 0) return 0;

    db->head = mergeSort(db->head, query);

    RecordNode *current = db->head;
    while (current && current->next) {
        current = current->next;
    }
    db->tail = current;

    writeCountOfAffectedRecordsToFile(SORT, db->size);
    return 1;
}

int validateFieldsAndConditions(Query *query) {
    if (!query) return 0;

    for (int i = 0; i < query->field_count; i++) {
        const QueryField field = query->fields[i];

        if (!validateKey(field.field)) {
            printf("Invalid field: %s\n", field.field);
            return 0;
        }

        if (query->action.value != SELECT && query->action.value != UNIQUE && query->action.value != SORT) {
            if (!validateValue(field.field, field.value)) {
                printf("Invalid value for field: %s\n", field.field);
                return 0;
            }
        }
    }

    for (int i = 0; i < query->condition_count; i++) {
        const Condition condition = query->conditions[i];

        if (!validateKey(condition.field)) {
            printf("Invalid condition: %s\n", condition.field);
            return 0;
        }

        if (!validateValue(condition.field, condition.value)) {
            printf("Invalid value for condition: %s\n", condition.field);
            return 0;
        }
    }

    return 1;
}

int execute(Database *database, Query *query) {
    if (!database || !query || !validateFieldsAndConditions(query)) return 0;

    if (query->action.value == INSERT) {
        return makeInsertQuery(database, query);
    }
    if (query->action.value == DELETE) {
        return makeDeleteQuery(database, query);
    }
    if (query->action.value == UPDATE) {
        return makeUpdateQuery(database, query);
    }
    if (query->action.value == SELECT) {
        return makeSelectQuery(database, query);
    }
    if (query->action.value == UNIQUE) {
        return makeUniqQuery(database, query);
    }
    if (query->action.value == SORT) {
        return makeSortQuery(database, query);
    }

    return 0;
}
