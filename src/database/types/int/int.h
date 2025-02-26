#ifndef INT_H
#define INT_H

#include "../../../database/types/query/comparison/comparison.h"

typedef struct CustomInt {
    int value;
    char *field;

    char *(*toString)(struct CustomInt *self);
    int (*update)(struct CustomInt *self, char *newValue);
    int (*compare)(struct CustomInt *self, char *other, ComparisonOptionEnum option);
} CustomInt;

CustomInt *intFactory(char *intString, const char *field);

#endif
