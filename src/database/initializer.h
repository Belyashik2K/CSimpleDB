#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "types/query/query.h"
#include "types/record/record.h"

typedef struct Database {
    RecordNode *head;
    RecordNode *tail;
    int size;

    int (*execute)(struct Database *self, Query *query);
} Database;

Database *initializeDatabase();
void freeDatabase(Database *database);

#endif
