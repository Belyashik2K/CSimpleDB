#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../time/time.h"

char *timeToString(const Time *self) {
    const int bufSize = strlen(self->field) + 50;
    char *buffer = malloc(bufSize);
    if (!buffer) return NULL;
    snprintf(buffer, bufSize, "%s='%02d:%02d:%02d'", self->field, self->hour, self->minute, self->second);
    return buffer;
}

Time *timeFactory(const char *timeString, const char *field) {
    int hour, minute, second;

    if (sscanf(timeString, "'%d:%d:%d'", &hour, &minute, &second) != 3 ||
        strlen(timeString) > 10)
        return NULL;

    if (hour < 0 || hour > 23) return NULL;
    if (minute < 0 || minute > 59) return NULL;
    if (second < 0 || second > 59) return NULL;

    Time *time = malloc(sizeof(Time));
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

    return time;
}
