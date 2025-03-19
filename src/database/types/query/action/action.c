#include <string.h>
#include <stdlib.h>

#include "../action/action.h"

#include "../../../../utils/mem_profiler/helper.h"

const char *actionStrings[] = {
    "insert",
    "delete",
    "update",
    "select",
    "uniq",
    "sort"
};

void freeAction(Action *action) {
    if (!action) return;
    freeWrapper(action);
}

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
    if (actionEnum == ACTION_COUNT) {
        return NULL;
    }

    Action *action = (Action *) mallocWrapper(sizeof(Action));
    if (!action) return NULL;
    action->value = actionEnum;
    return action;
}