#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int mallocCount = 0;
extern int callocCount = 0;
extern int reallocCount = 0;
extern int strdupCount = 0;
extern int freeCount = 0;

void *mallocWrapper(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) return NULL;

    mallocCount++;
    return ptr;
}

void *callocWrapper(size_t num, size_t size) {
    void *ptr = calloc(num, size);
    if (ptr == NULL) return NULL;

    callocCount++;
    return ptr;
}

void *reallocWrapper(void *ptr, size_t size) {
    if (ptr == NULL) return mallocWrapper(size);

    void *newPtr = realloc(ptr, size);
    if (newPtr == NULL) return NULL;

    reallocCount++;
    return newPtr;
}

char *strdupWrapper(const char *str) {
    if (str == NULL) return NULL;

    char *result = strdup(str);
    if (result == NULL) return NULL;

    strdupCount++;
    return result;
}

void _freeWrapper(void **ptr) {
    if (ptr == NULL || *ptr == NULL) return;

    free(*ptr);
    *ptr = NULL;
    freeCount++;
}
