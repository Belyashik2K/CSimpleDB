#ifndef TIME_H
#define TIME_H

#include "../../../database/types/query/comparison/comparison.h"

typedef struct Time {
    char hour;
    char minute;
    char second;
    char *field;

    char *(*toString)(struct Time *self);
    int (*update)(struct Time *self, char *newValue);
    int (*compare)(struct Time *self, char *other, ComparisonOptionEnum option);
} Time;

Time *timeFactory(const char *timeString, const char *field);
void freeTime(Time *time);

#endif
