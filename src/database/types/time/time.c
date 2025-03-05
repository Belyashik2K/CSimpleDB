#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../time/time.h"

int equalTime(Time *self, char *other) {
    Time *otherTime = timeFactory(other, self->field);
    if (!otherTime) return 0;

    return self->hour == otherTime->hour && self->minute == otherTime->minute && self->second == otherTime->second;
}

int notEqualTime(Time *self, char *other) {
    return !equalTime(self, other);
}

int lessTime(Time *self, char *other) {
    Time *otherTime = timeFactory(other, self->field);
    if (!otherTime) {
        return 0;
    }

    if (self->hour < otherTime->hour) return 1;
    if (self->hour > otherTime->hour) return 0;

    if (self->minute < otherTime->minute) return 1;
    if (self->minute > otherTime->minute) return 0;

    if (self->second < otherTime->second) return 1;
    if (self->second > otherTime->second) return 0;

    return 0;
}

int greaterTime(Time *self, char *other) {
    return !lessTime(self, other) && notEqualTime(self, other);
}

int lessOrEqualTime(Time *self, char *other) {
    return lessTime(self, other) || equalTime(self, other);
}

int greaterOrEqualTime(Time *self, char *other) {
    return greaterTime(self, other) || equalTime(self, other);
}

int compareTimes(Time *self, char *other, ComparisonOptionEnum option) {
    switch (option) {
        case EQUAL:
            return equalTime(self, other);
        case NOT_EQUAL:
            return notEqualTime(self, other);
        case LESS:
            return lessTime(self, other);
        case GREATER:
            return greaterTime(self, other);
        case LESS_OR_EQUAL:
            return lessOrEqualTime(self, other);
        case GREATER_OR_EQUAL:
            return greaterOrEqualTime(self, other);
        default:
            return 0;
    }
}

char *timeToString(Time *self) {
    const int bufSize = strlen(self->field) + 50;
    char *buffer = (char *) malloc(bufSize);
    if (!buffer) return NULL;
    snprintf(buffer, bufSize, "%s='%02d:%02d:%02d'", self->field, self->hour, self->minute, self->second);
    return buffer;
}

int updateTime(Time *self, char *newValue) {
    Time *newTime = timeFactory(newValue, self->field);
    if (!newTime) return 0;

    self->hour = newTime->hour;
    self->minute = newTime->minute;
    self->second = newTime->second;

    return 1;
}

Time *timeFactory(const char *timeString, const char *field) {
    int hour, minute, second;

    if (sscanf(timeString, "'%d:%d:%d'", &hour, &minute, &second) != 3 ||
        strlen(timeString) > 10)
        return NULL;

    if (hour < 0 || hour > 23) return NULL;
    if (minute < 0 || minute > 59) return NULL;
    if (second < 0 || second > 59) return NULL;

    Time *time = (Time *) malloc(sizeof(Time));
    if (time == NULL) return NULL;

    time->hour = (char) hour;
    time->minute = (char) minute;
    time->second = (char) second;

    time->field = strdup(field);
    if (!time->field) {
        free(time);
        return NULL;
    }

    time->toString = timeToString;
    time->compare = compareTimes;
    time->update = updateTime;

    return time;
}
