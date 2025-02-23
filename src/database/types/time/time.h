#ifndef TIME_H
#define TIME_H

typedef struct Time {
    char hour;
    char minute;
    char second;
    char *field;

    char *(*toString)(struct Time *self);
} Time;

Time *timeFactory(const char *timeString, const char *field);

#endif
