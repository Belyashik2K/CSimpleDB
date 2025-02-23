#include <stdio.h>

#include "database/initializer.h"
#include "utils/parser/file_parser.h"

int main(void) {

    Database *database = initializeDatabase();
    if (!database) {
        printf("Database initialization failed, maybe you are out of memory?\n");
        return 1;
    }

    const int parsed = parseFile("D:/test.txt", database);
    if (!parsed) {
        printf("Parsing failed, maybe the file does not exist?\n");
        freeDatabase(database);
        return 1;
    }

    freeDatabase(database);
    return 0;
}
