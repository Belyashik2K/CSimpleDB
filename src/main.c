#include <stdio.h>

#include "database/initializer.h"
#include "utils/parser/file_parser.h"
#include "utils/writer/file_writer.h"

int main(void) {

    prepareOutputFile();

    Database *database = initializeDatabase();
    if (!database) {
        printf("Database initialization failed, maybe you are out of memory?\n");
        return 1;
    }
    printf("Database initialized, starting to parse the file...\n");

    const int parsed = parseFile("C:/test.txt", database);
    if (!parsed) {
        printf("Parsing failed, maybe the file does not exist?\n");
        freeDatabase(database);
        return 1;
    }

    freeDatabase(database);
    printf("Database freed, exiting...\n");
    return 0;
}
