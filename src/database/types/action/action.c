#include <string.h>

#include "../action/action.h"

#include <stdio.h>

const char *action_strings[] = {
    "insert",
    "delete",
    "update",
    "select"
};

const char *action_to_string(Action action) {
    if (action >= ACTION_COUNT) {
        return "unknown";
    }
    return action_strings[action];
}

Action action_from_string(const char *str) {
    for (int i = 0; i < ACTION_COUNT; i++) {
        if (strcmp(str, action_strings[i]) == 0) {
            return (Action) i;
        }
    }
    return ACTION_COUNT;
}