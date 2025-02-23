#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "../../database/types/action/action.h"

void prepareOutputFile();
void writeCountOfAffectedRecordsToFile(Action action, int count);
void writeIncorrectLineToFile(const char *line);

#endif
