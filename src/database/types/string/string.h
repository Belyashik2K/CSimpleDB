#ifndef STRING_H
#define STRING_H

typedef struct CustomString {
    char *value;
    char *field;

    char *(*toString)(struct CustomString *self);
} CustomString;

CustomString *stringFactory(char *string, const char *field);

#endif
