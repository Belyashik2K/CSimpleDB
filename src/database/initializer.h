#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "types/record/record.h"

typedef struct {
    RecordNode *head;
    RecordNode *tail;
    int size;
} Database;

Database *initializeDatabase();
void freeDatabase(Database *database);

#endif
