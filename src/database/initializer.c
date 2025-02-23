#include <stdlib.h>

#include "../database/initializer.h"

#include <stdio.h>

Database *initializeDatabase() {
    Database *database = malloc(sizeof(Database));
    if (!database) return NULL;
    database->head = NULL;
    database->tail = NULL;
    database->size = 0;

    return database;
}

void freeDatabase(Database *database) {
    if (!database) return;

    RecordNode *current = database->head;
    while (current) {
        RecordNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(database);
}
