#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "initializer.h"
#include "types/record/record.h"

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
    return 1;
}

int makeSelectQuery(Database *db, Query *query) {
    if (!db || !query || !db->head) return 0;

    const RecordNode *current = db->head;
    while (current) {
        if (query->condition_count) {
            int not_satisfy = 0;
            for (int i = 0; i < query->condition_count; i++) {
                if (!compareMeaDate(
                        &current->data->mea_date,
                        query->conditions[i].value,
                        query->conditions[i].comparison->operator
                    )
                ) {
                    not_satisfy = 1;
                }
            }
            if (not_satisfy) {
                current = current->next;
                continue;
            }
        }

        for (int i = 0; i < query->field_count; i++) {
            const QueryField field = query->fields[i];
            printKey(field.field, current->data);
            printf(" ");
        }
        printf("\n");
        current = current->next;
    }

    return 1;
}

int makeDeleteQuery(Database *db, Query *query) {
    if (!db || !query) return 0;

    return 1;
}

int makeUpdateQuery(Database *db, Query *query) {
    if (!db || !query) return 0;

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

        if (query->action.value == SELECT) {
            break;
        }

        if (!validateValue(field.field, field.value)) {
            printf("Invalid value for field: %s\n", field.field);
            return 0;
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

    return 0;
}

void printDatabase(Database *db) {
    if (!db || !db->head) {
        printf("Database is empty.\n");
        return;
    }

    const RecordNode *current = db->head;
    while (current) {
        Record *r = current->data;
        printf("Record: %s %s %s %s %s %s %s\n",
               r->geo_id.toString(&r->geo_id),
               r->geo_pos.toString(&r->geo_pos),
               r->mea_date.toString(&r->mea_date),
               r->level.toString(&r->level),
               r->sunrise.toString(&r->sunrise),
               r->weather.toString(&r->weather),
               r->sundown.toString(&r->sundown)
        );

        current = current->next;
    }
}
