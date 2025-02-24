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

int execute(Database *database, Query *query) {
    if (!database || !query) return 0;
    if (query->action.value != INSERT) return 0;

    if (query->action.value == INSERT) {
        return makeInsertQuery(database, query);
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
