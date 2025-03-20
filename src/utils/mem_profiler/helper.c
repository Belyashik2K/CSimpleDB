#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mallocCount = 0;
int callocCount = 0;
int reallocCount = 0;
int strdupCount = 0;
int freeCount = 0;

void *mallocWrapper(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }

    mallocCount++;
    return ptr;
}

void *callocWrapper(size_t num, size_t size) {
    void *ptr = calloc(num, size);
    if (ptr == NULL) {
        return NULL;
    }

    callocCount++;
    return ptr;
}

void *reallocWrapper(void *ptr, size_t size) {
    if (ptr == NULL) return mallocWrapper(size);
    if (size == 0) {
        freeWrapper(ptr);
        return NULL;
    }

    void *newPtr = realloc(ptr, size);
    if (newPtr == NULL) return ptr;

    reallocCount++;
    return newPtr;
}

char *strdupWrapper(const char *str) {
    if (str == NULL) return NULL;

    char *result = strdup(str);
    if (result == NULL) {
        printf("strdup failed\n");
        return NULL;
    }

    strdupCount++;
    return result;
}

void _freeWrapper(void **ptr) {
    if (ptr == NULL || *ptr == NULL) {
        return;
    }

    free(*ptr);
    *ptr = NULL;
    freeCount++;
}
