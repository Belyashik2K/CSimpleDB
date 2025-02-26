#ifndef ACTION_H
#define ACTION_H

typedef enum {
    INSERT,
    DELETE,
    UPDATE,
    SELECT,
    UNIQUE,
    ACTION_COUNT
} ActionEnum;

typedef struct Action {
    ActionEnum value;
} Action;

Action *actionFactory(char *actionString);

#endif
