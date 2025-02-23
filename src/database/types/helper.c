#include <stdlib.h>

#include "../initializer.h"
#include "./record/record.h"

void generateRecord() {
    ;
}

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
