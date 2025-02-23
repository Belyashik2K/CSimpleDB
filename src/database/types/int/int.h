#ifndef INT_H
#define INT_H

typedef struct {
    int value;
} CustomInt;

CustomInt *intFactory(char *intString);

#endif
