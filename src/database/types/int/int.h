#ifndef INT_H
#define INT_H

typedef struct CustomInt {
    int value;
    char *field;

    char *(*toString)(struct CustomInt *self);
} CustomInt;

CustomInt *intFactory(char *intString, const char *field);

#endif
