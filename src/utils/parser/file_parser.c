#include <stdio.h>

#include "../../database/initializer.h"
#include "../../database/types/query/query.h"
#include "../../utils/writer/file_writer.h"

#define MAX_LINE_LENGTH 65536

int parseFile(const char *filename, Database *database) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        // printf("Parsing query...\n");
        Query *query = queryFactory(line);
        // if (query) {
        //     printf("Query parsed successfully.\n");
        //     for (int i = 0; i < query->field_count; i++) {
        //         printf("Field: %s, Value: %s\n", query->fields[i].field, query->fields[i].value);
        //     }
        //     for (int i = 0; i < query->condition_count; i++) {
        //         printf("Condition: %s %d %s\n", query->conditions[i].field, query->conditions[i].comparison->operator,
        //                query->conditions[i].value);
        //     }
        // }
        const int executionResult = database->execute(database, query);
        if (!executionResult) {
            writeIncorrectLineToFile(line);
        }
        freeQuery(query);
    }

    // printf("\nDatabase after parsing:\n");
    // printDatabase(database);

    fclose(file);

    return 1;
}
