#ifndef DATE_H
#define DATE_H

typedef struct Date {
    char day;
    char month;
    short year;
    char *field;

    char *(*toString)(struct Date *self);
} Date;

Date *dateFactory(const char *dateString, const char *field);

#endif
