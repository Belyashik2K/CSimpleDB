#ifndef ACTION_H
#define ACTION_H

typedef enum {
    INSERT,
    DELETE,
    UPDATE,
    SELECT,
    ACTION_COUNT
} Action;

extern const char *action_strings[];

const char *action_to_string(Action action);
Action action_from_string(const char *str);

#endif
