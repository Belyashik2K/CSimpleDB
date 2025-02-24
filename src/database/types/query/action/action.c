#include <string.h>
#include <stdlib.h>

#include "../action/action.h"

const char *actionStrings[] = {
    "insert",
    "delete",
    "update",
    "select"
};

ActionEnum actionFromString(const char *str) {
    for (int i = 0; i < ACTION_COUNT; i++) {
        if (strcmp(str, actionStrings[i]) == 0) {
            return (ActionEnum) i;
        }
    }
    return ACTION_COUNT;
}

Action *actionFactory(char *actionString) {
    const ActionEnum actionEnum = actionFromString(actionString);
    if (actionFromString(actionString) == ACTION_COUNT) {
        return NULL;
    }

    Action *action = malloc(sizeof(Action));
    if (!action) return NULL;
    action->value = actionEnum;
    return action;
}