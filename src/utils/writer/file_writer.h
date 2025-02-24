#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "../../database/types/query/action/action.h"

void prepareOutputFile();
void writeCountOfAffectedRecordsToFile(ActionEnum action, int count);
void writeIncorrectLineToFile(const char *line);

#endif
