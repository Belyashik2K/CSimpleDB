#ifndef STRING_H
#define STRING_H

#include "../../../database/types/query/comparison/comparison.h"

typedef struct CustomString {
    char *value;
    char *field;

    char *(*toString)(struct CustomString *self);
    int (*update)(struct CustomString *self, char *newValue);
    int (*compare)(struct CustomString *self, char *other, ComparisonOptionEnum option);
} CustomString;

CustomString *stringFactory(char *string, const char *field);
void freeString(CustomString *customString);
int updateString(CustomString *self, char *newValue);

#endif
