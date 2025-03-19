#include <stdlib.h>

#include "../database/initializer.h"
#include "../database/helper.h"
#include "../utils/mem_profiler/helper.h"

Database *initializeDatabase() {
    Database *database = (Database *) malloc(sizeof(Database));
    if (!database) return NULL;
    database->head = NULL;
    database->tail = NULL;
    database->size = 0;
    database->execute = execute;

    return database;
}

void freeDatabase(Database *database) {
    if (!database) return;

    RecordNode *current = database->head;
    while (current) {
        RecordNode *next = current->next;
        freeRecordNode(current);
        current = next;
        freeWrapper(current);
    }
}
