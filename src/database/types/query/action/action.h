#ifndef ACTION_H
#define ACTION_H

typedef enum {
    INSERT,
    DELETE,
    UPDATE,
    SELECT,
    UNIQUE,
    SORT,
    ACTION_COUNT
} ActionEnum;

typedef struct Action {
    ActionEnum value;
} Action;

Action *actionFactory(char *actionString);
void freeAction(Action *action);

#endif
