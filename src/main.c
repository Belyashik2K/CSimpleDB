#include <stdio.h>

#include "database/initializer.h"
#include "utils/parser/file_parser.h"
#include "utils/writer/file_writer.h"

#include "database/types/date/date.h"
#include "database/types/int/int.h"
#include "database/types/string/string.h"
#include "database/types/time/time.h"

#include "utils/mem_profiler/helper.h"

#define INPUT_FILE "../tests/uniq.txt"

int main(void) {
    prepareOutputFile();

    // Date *date = dateFactory("'01.01.2021'", "date");
    // if (!date) {
    //     printf("Date factory failed\n");
    //     return 1;
    // }
    // freeDate(date);
    //
    // CustomInt *customInt = intFactory("123", "int");
    // freeWrapper(customInt->field);
    // if (!customInt) {
    //     printf("Int factory failed\n");
    //     return 1;
    // }
    // freeInt(customInt);
    //
    // CustomString *customString = stringFactory("\"string\"", "string");
    // if (!customString) {
    //     printf("String factory failed\n");
    //     return 1;
    // }
    // printf("String: %s\n", customString->value);
    //
    // // Trying to update the string
    // int updateResult = updateString(customString, "\"new string\"");
    // if (!updateResult) {
    //     printf("String update failed\n");
    //     return 1;
    // }
    // printf("Updated string: %s\n", customString->value);
    //
    // // Checking if given string is invalid
    // int unsuccessfulUpdateResult = updateString(customString, "invalid");
    // if (unsuccessfulUpdateResult) {
    //     printf("String update should have failed\n");
    //     return 1;
    // }
    // printf("String update failed as expected\n");
    //
    // freeString(customString);
    //
    // Time *time = timeFactory("'12:34:56'", "time");
    // if (!time) {
    //     printf("Time factory failed\n");
    //     return 1;
    // }


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
