#include <stdio.h>
#include <string.h>

#include "../writer/file_writer.h"
#include "../../database/helper.h"
#include "../../database/initializer.h"
#include "../../database/types/query/action/action.h"
#include "../../database/types/query/query.h"

#define MAX_LINE_LENGTH 1024

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
        printf("Parsing query...\n");
        Query *query = queryFactory(line);
        if (query) {
            printf("Query parsed successfully.\n");
        }
        const int result = database->execute(database, query);
        if (!result) {
            printf("Query execution failed.\n");
        }
        printf("\n");
    }

    printf("\nDatabase after parsing:\n");
    printDatabase(database);

    fclose(file);

    return 1;
}
