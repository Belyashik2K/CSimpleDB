#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../date/date.h"


int is_leap_year(const int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int is_valid_date(
    const int day,
    const int month,
    const int year
) {
    if (year < 1000 || year > 9999) {
        return 0;
    }

    if (month < 1 || month > 12) {
        return 0;
    }

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && is_leap_year(year)) {
        days_in_month[1] = 29;
    }

    if (day < 1 || day > days_in_month[month - 1]) {
        return 0;
    }

    return 1;
}

Date *dateFactory(const char *dateString) {
    int day, month, year;

    if (
        sscanf(dateString, "'%d.%d.%d'", &day, &month, &year) != 3 ||
        strlen(dateString) > 12
    )
        return NULL;

    if (!is_valid_date(day, month, year)) return NULL;

    Date *date = malloc(sizeof(Date));
    if (date == NULL) return NULL;

    date->day = (char) day;
    date->month = (char) month;
    date->year = (short) year;

    return date;
}
