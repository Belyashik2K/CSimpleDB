#ifndef CONDITION_H
#define CONDITION_H

#include "../comparison/comparison.h"

typedef struct {
    char *field;
    Comparison *comparison;
    char *value;
} Condition;

Condition conditionFactory(char *conditionString);

#endif
