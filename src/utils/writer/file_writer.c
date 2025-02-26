#include "file_writer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../database/types/query/action/action.h"
#include "../../database/types/record/record.h"

#define OUTPUT_FILE "../output.txt"

void prepareOutputFile() {
    FILE *file = fopen(OUTPUT_FILE, "w");
    if (!file) return;
    fclose(file);
}

char *prepareSelectResult(Record *record, Query *query) {
    char *result = calloc(1, sizeof(char));
    if (!result) {
        printf("Could not allocate memory\n");
        return NULL;
    }
    for (int i = 0; i < query->field_count; i++) {
        const QueryField field = query->fields[i];
        char *fieldString = getFieldStringRepresentation(field.field, record);

        if (!fieldString) {
            free(result);
            return NULL;
        }

        char *new_result = realloc(result, strlen(result) + strlen(fieldString) + 2);
        if (!new_result) {
            printf("Could not reallocate memory\n");
            free(result);
            free(fieldString);
            return NULL;
        }

        result = new_result;
        strcat(result, fieldString);
        if (i < query->field_count - 1) {
            strcat(result, " ");
        }
        free(fieldString);
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
        default:
            break;
    }

    fclose(file);

}

void writeSelectResultToFile(Record *record, Query *query) {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (!file) {
        printf("Could not open file for writing\n");
    }

    char *result = prepareSelectResult(record, query);
    if (!result) {
        printf("Could not prepare select result\n");
        return;
    }

    fprintf(file, "%s\n", result);
    free(result);

    fclose(file);
}

void writeIncorrectLineToFile(const char *line) {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (!file) return;

    fprintf(file, "incorrect:'%.20s'\n", line);
}