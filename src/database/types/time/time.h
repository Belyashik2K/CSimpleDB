#ifndef TIME_H
#define TIME_H

typedef struct {
    char hour;
    char minute;
    char second;
} Time;

Time *timeFactory(const char *timeString);

#endif