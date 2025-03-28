#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../date/date.h"
#include "../query/comparison/comparison.h".
#include "../../../utils/mem_profiler/helper.h"

void freeDate(Date *date) {
    if (!date) return;

    freeWrapper(date->field);
    freeWrapper(date);
}

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

char *dateToString(Date *self) {
    const int bufSize = strlen(self->field) + 50;
    char *buffer = (char *) mallocWrapper(bufSize);
    if (!buffer) return NULL;
    sprintf(buffer, "%s='%02d.%02d.%02d'", self->field, self->day, self->month, self->year);
    return buffer;
}

int equalDate(const Date *self, char *other) {
    Date *otherDate = dateFactory(other, self->field);
    if (!otherDate) return 0;

    int result = self->day == otherDate->day && self->month == otherDate->month && self->year == otherDate->year;
    freeDate(otherDate);

    return result;
}

int notEqualDate(const Date *self, char *other) {
    return !equalDate(self, other);
}

int lessDate(const Date *self, const char *other) {
    Date *otherDate = dateFactory(other, self->field);
    if (!otherDate) {
        return 0;
    }

    int result = 0;
    if (self->year != otherDate->year)
        result = self->year < otherDate->year;
    else if (self->month != otherDate->month)
        result = self->month < otherDate->month;
    else
        result = self->day < otherDate->day;

    freeDate(otherDate);
    return result;
}

int greaterDate(const Date *self, char *other) {
    return !lessDate(self, other) && notEqualDate(self, other);
}

int lessOrEqualDate(const Date *self, char *other) {
    return lessDate(self, other) || equalDate(self, other);
}

int greaterOrEqualDate(const Date *self, char *other) {
    return greaterDate(self, other) || equalDate(self, other);
}

int compareDates(Date *self, char *other, ComparisonOptionEnum option) {
    switch (option) {
        case EQUAL:
            return equalDate(self, other);
        case NOT_EQUAL:
            return notEqualDate(self, other);
        case LESS:
            return lessDate(self, other);
        case GREATER:
            return greaterDate(self, other);
        case LESS_OR_EQUAL:
            return lessOrEqualDate(self, other);
        case GREATER_OR_EQUAL:
            return greaterOrEqualDate(self, other);
        default:
            return 0;
    }
}

int updateDate(Date *self, char *newValue) {
    Date *newDate = dateFactory(newValue, self->field);
    if (!newDate) return 0;

    self->day = newDate->day;
    self->month = newDate->month;
    self->year = newDate->year;

    freeDate(newDate);
    return 1;
}

Date *dateFactory(const char *dateString, const char *field) {
    int day, month, year;

    if (
        sscanf(dateString, "'%d.%d.%d'", &day, &month, &year) != 3 ||
        strlen(dateString) > 12
    )
        return NULL;

    if (!is_valid_date(day, month, year)) return NULL;

    Date *date = (Date *) mallocWrapper(sizeof(Date));
    if (date == NULL) return NULL;

    date->day = (char) day;
    date->month = (char) month;
    date->year = (short) year;

    date->field = strdupWrapper(field);
    if (!date->field) {
        freeDate(date);
        return NULL;
    }

    date->toString = dateToString;
    date->compare = compareDates;
    date->update = updateDate;

    return date;
}
