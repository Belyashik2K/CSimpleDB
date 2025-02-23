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
        const Record *r = current->data;
        printf("Record: %d, %s, '%02d.%02d.%04d', %d, '%02d:%02d:%02d', %s, '%02d:%02d:%02d'\n",
               r->geo_id.value,
               r->geo_pos.value,
               r->mea_date.day,
               r->mea_date.month,
               r->mea_date.year,
               r->level.value,
               r->sunrise.hour,
               r->sunrise.minute,
               r->sunrise.second,
               weather_to_string(r->weather),
               r->sundown.hour,
               r->sundown.minute,
               r->sundown.second
        );
        current = current->next;
    }
}