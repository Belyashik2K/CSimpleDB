#include "file_writer.h"

#include <stdio.h>

#include "../../database/types/action/action.h"

#define OUTPUT_FILE "../output.txt"

void prepareOutputFile() {
    FILE *file = fopen(OUTPUT_FILE, "w");
    if (!file) return;
    fclose(file);
}

void writeCountOfAffectedRecordsToFile(const Action action, const int count) {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (!file) return;

    switch (action) {
        case INSERT:
            fprintf(file, "insert:%d\n", count);
            break;
        case DELETE:
            fprintf(file, "delete:%d\n", count);
            break;
        case UPDATE:
            fprintf(file, "update:%d\n", count);
            break;
        case SELECT:
            fprintf(file, "select:%d\n", count);
            break;
        default:
            break;
    }

}

void writeIncorrectLineToFile(const char *line) {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (!file) return;

    fprintf(file, "incorrect:'%.20s'\n", line);
}