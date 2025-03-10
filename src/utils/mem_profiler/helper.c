#include "helper.h"

#include <stdlib.h>
#include <string.h>

extern int mallocCount = 0;
extern int callocCount = 0;
extern int reallocCount = 0;
extern int strdupCount = 0;
extern int freeCount = 0;

void *mallocWrapper(size_t size) {
    mallocCount++;
    void *ptr = malloc(size);
    return ptr;
}

void *callocWrapper(size_t num, size_t size) {
    callocCount++;
    void *ptr = calloc(num, size);
    return ptr;
}

void *reallocWrapper(void *ptr, size_t size) {
    reallocCount++;
    return realloc(ptr, size);
}

char *strdupWrapper(const char *str) {
    if (str == NULL) return NULL;
    strdupCount++;
    return strdup(str);
}

void freeWrapper(void **ptr) {
    if (ptr == NULL || *ptr == NULL) return;

    freeCount++;
    free(*ptr);
    *ptr = NULL;
}
