#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../time/time.h"

Time *timeFactory(const char *timeString) {
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

    return time;
}
