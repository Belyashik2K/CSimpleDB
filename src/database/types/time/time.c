#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../time/time.h"
#include "../../../utils/mem_profiler/helper.h"

void freeTime(Time *time) {
    if (!time) return;

    freeWrapper(time->field);
    freeWrapper(time);
}

int equalTime(Time *self, char *other) {
    Time *otherTime = timeFactory(other, self->field);
    if (!otherTime) return 0;

    int result = self->hour == otherTime->hour &&
                 self->minute == otherTime->minute &&
                 self->second == otherTime->second;
    freeTime(otherTime);

    return result;
}

int notEqualTime(Time *self, char *other) {
    return !equalTime(self, other);
}

int lessTime(Time *self, char *other) {
    Time *otherTime = timeFactory(other, self->field);
    if (!otherTime) {
        return 0;
    }

    int result = 0;
    if (self->hour != otherTime->hour)
        result = self->hour < otherTime->hour;
    else if (self->minute != otherTime->minute)
        result = self->minute < otherTime->minute;
    else
        result = self->second < otherTime->second;
    freeTime(otherTime);

    return result;
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
    char *buffer = (char *) mallocWrapper(bufSize);
    if (!buffer) return NULL;
    sprintf(buffer, "%s='%02d:%02d:%02d'", self->field, self->hour, self->minute, self->second);
    return buffer;
}

int updateTime(Time *self, char *newValue) {
    Time *newTime = timeFactory(newValue, self->field);
    if (!newTime) return 0;

    self->hour = newTime->hour;
    self->minute = newTime->minute;
    self->second = newTime->second;

    freeTime(newTime);

    return 1;
}

Time *timeFactory(const char *timeString, const char *field) {
    int hour, minute, second;

    if (sscanf(timeString, "'%d:%d:%d'", &hour, &minute, &second) != 3 ||
        strlen(timeString) > 10)
        return NULL;

    if (hour < 0 || hour > 23) return NULL;
    if (minute < 0 || minute > 59) return NULL;
    if (minute < 0 || minute > 59) return NULL;
    if (second < 0 || second > 59) return NULL;

    Time *time = (Time *) mallocWrapper(sizeof(Time));
    if (time == NULL) return NULL;

    time->hour = (char) hour;
    time->minute = (char) minute;
    time->second = (char) second;

    time->field = strdupWrapper(field);

    time->toString = timeToString;
    time->compare = compareTimes;
    time->update = updateTime;

    return time;
}
