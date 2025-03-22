#include "file_writer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../database/types/query/action/action.h"
#include "../../database/types/record/record.h"
#include "../mem_profiler/helper.h"

#define OUTPUT_FILE "../output.txt"
#define MEMSTAT_FILE "../memstat.txt"

void prepareOutputFile() {
    FILE *file = fopen(OUTPUT_FILE, "w");
    if (!file) return;
    fclose(file);
}

void prepareMemstatFile() {
    FILE *file = fopen(MEMSTAT_FILE, "w");
    if (!file) return;
    fclose(file);
}

char *prepareSelectResult(Record *record, Query *query) {
    char *result = (char *) callocWrapper(1, sizeof(char));
    if (!result) {
        return NULL;
    }

    for (int i = 0; i < query->field_count; i++) {
        QueryField *field = query->fields[i];

        char *fieldString = getFieldStringRepresentation(field->field, record);
        if (!fieldString) {
            freeWrapper(result);
            return NULL;
        }

        result = (char *) reallocWrapper(result, strlen(result) + strlen(fieldString) + 2);
        strcat(result, fieldString);
        if (i < query->field_count - 1) {
            strcat(result, " ");
        }

        freeWrapper(fieldString);
    }

    return result;
}

void writeCountOfAffectedRecordsToFile(const ActionEnum action, const int count) {
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
        case UNIQUE:
            fprintf(file, "uniq:%d\n", count);
            break;
        case SORT:
            fprintf(file, "sort:%d\n", count);
            break;
        default:
            break;
    }

    fclose(file);

}

void writeSelectResultToFile(Record *record, Query *query) {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (!file) {
        return;
    }

    char *result = prepareSelectResult(record, query);
    if (!result) {
        fclose(file);
        return;
    }

    fprintf(file, "%s\n", result);
    freeWrapper(result);
    fclose(file);
}

void writeIncorrectLineToFile(const char *line) {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (!file) return;

    fprintf(file, "incorrect:'%.20s'\n", line);
    fclose(file);
}

void writeMemstat() {
    FILE *file = fopen(MEMSTAT_FILE, "w");
    if (!file) return;

    fprintf(file, "malloc:%d\n", mallocCount);
    fprintf(file, "calloc:%d\n", callocCount);
    fprintf(file, "realloc:%d\n", reallocCount);
    fprintf(file, "strdup:%d\n", strdupCount);
    fprintf(file, "free:%d\n", freeCount);

    fprintf(file, "not unloaded:%d\n", mallocCount + callocCount + strdupCount - freeCount);

    fclose(file);
}