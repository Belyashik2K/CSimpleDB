#ifndef STRING_H
#define STRING_H

typedef struct {
    char *value;
} CustomString;

CustomString *stringFactory(char *string);

#endif
