#include <stdio.h>
#include <stdlib.h>

#include "initializer.h"
#include "types/record/record.h"

void appendRecord(Database *db, Record *record) {
    if (!db || !record) return;

    RecordNode *newNode = malloc(sizeof(RecordNode));
    if (!newNode) return;

    newNode->data = record;
    newNode->next = NULL;

    if (db->tail) {
        db->tail->next = newNode;
    } else {
        db->head = newNode;
    }
    db->tail = newNode;
    db->size++;
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
