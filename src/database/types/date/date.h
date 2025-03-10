#ifndef DATE_H
#define DATE_H

#include "../query/comparison/comparison.h"

typedef struct Date {
    char day;
    char month;
    short year;
    char *field;

    char *(*toString)(struct Date *self);
    int (*update)(struct Date *self, char *newValue);
    int (*compare)(struct Date *self, char *other, ComparisonOptionEnum option);
} Date;

Date *dateFactory(const char *dateString, const char *field);
void freeDate(Date *date);

#endif
