#include <string.h>
#include <stdlib.h>

#include "../action/action.h"

const char *actionStrings[] = {
    "insert",
    "delete",
    "update",
    "select",
    "uniq",
    "sort"
};

ActionEnum actionFromString(char *str) {
    str[strcspn(str, "\n")] = 0;
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

    Action *action = (Action *) malloc(sizeof(Action));
    if (!action) return NULL;
    action->value = actionEnum;
    return action;
}