#include <stdio.h>
#include <string.h>

#include "../../database/initializer.h"
#include "../../database/helper.h"
#include "../../database/types/action/action.h"

#define MAX_LINE_LENGTH 1024

typedef int (*parserFunc)(char *, Database *);

int insertParser(char *line, Database *database) {
    Record *record = recordFactory(line);
    if (!record) {
        return 0;
    }
    appendRecord(database, record);
    return 1;
}

int deleteParser(char *line, Database *database) {
    return 1;
}

int selectParser(char *line, Database *database) {
    return 1;
}

int updateParser(char *line, Database *database) {
    return 1;
}

int incorrectParser(char *line, Database *database) {
    return 0;
}

typedef struct {
    Action key;
    parserFunc func;
} KeyMap;

static KeyMap keyMappings[] = {
    {INSERT, insertParser},
    {DELETE, deleteParser},
    {SELECT, selectParser},
    {UPDATE, updateParser},
    {ACTION_COUNT, incorrectParser}
};

char *findAction(const char *line) {
    char *lineCopy = strdup(line);
    lineCopy[6] = '\0';
    return lineCopy;
}

void redirectToConcreteParser(char *line, Database *database) {
    const Action actionToEnum = action_from_string(findAction(line));
    for (int i = 0; i < ACTION_COUNT; i++) {
        if (actionToEnum == keyMappings[i].key) {
            keyMappings[i].func(line, database);
            break;
        }
    }
}

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
        redirectToConcreteParser(line, database);
    }

    printDatabase(database);

    fclose(file);

    return 1;
}
