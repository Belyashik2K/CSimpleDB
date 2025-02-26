#ifndef HELPER_H
#define HELPER_H

#include "initializer.h"

// void appendRecord(Database *db, Record *record);
int execute(Database *database, Query *query);
void printDatabase(Database *db);

#endif
