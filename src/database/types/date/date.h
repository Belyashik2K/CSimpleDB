#ifndef DATE_H
#define DATE_H

typedef struct Date {
    char day;
    char month;
    short year;
    char *field;

    char *(*toString)(struct Date *self);
} Date;


// int is_leap_year(int year);
// int is_valid_date(int day, int month, int year);
Date *dateFactory(const char *dateString, const char *field);

#endif
