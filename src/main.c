#include <stdio.h>

#include "database/initializer.h"
#include "utils/parser/file_parser.h"
#include "utils/writer/file_writer.h"

#define INPUT_FILE "../tests/select.txt"

int main(void) {

    prepareOutputFile();

    Database *database = initializeDatabase();
    if (!database) {
        printf("Database initialization failed, maybe you are out of memory?\n");
        return 1;
    }
    printf("Database initialized, starting to parse the file...\n");

    const int parsed = parseFile(INPUT_FILE, database);
    if (!parsed) {
        printf("Parsing failed, maybe the file does not exist?\n");
        freeDatabase(database);
        return 1;
    }

    printf("File parsed, freeing the database...\n");
    freeDatabase(database);
    printf("Database freed...\n");
    printf("Writing memstat.txt...\n");
    writeMemstat();
    printf("memstat.txt written, exiting...\n");
    return 0;
}
