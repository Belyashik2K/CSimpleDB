#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "../../database/types/record/record.h"
#include "../../database/types/query/action/action.h"

void prepareOutputFile();
void prepareMemstatFile();
void writeCountOfAffectedRecordsToFile(ActionEnum action, int count);
void writeIncorrectLineToFile(const char *line);
void writeSelectResultToFile(Record *record, Query *query);
void writeMemstat();

#endif
