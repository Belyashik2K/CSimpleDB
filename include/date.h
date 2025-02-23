#ifndef DATE_H
#define DATE_H

typedef struct {
    char day;
    char month;
    short year;
} Date;


int is_leap_year(int year);
int is_valid_date(int day, int month, int year);
Date *dateFactory(const char *dateString);

#endif