//
// Created by Belyashik2K on 10.03.2025.
//

#ifndef HELPER2_H
#define HELPER2_H

#include <stddef.h>

#define freeWrapper(ptr) _freeWrapper((void **)&(ptr))

extern int mallocCount;
extern int callocCount;
extern int reallocCount;
extern int strdupCount;
extern int freeCount;

void *mallocWrapper(size_t size);

void *callocWrapper(size_t count, size_t size);

void *reallocWrapper(void *ptr, size_t size);

char *strdupWrapper(const char *str);

void _freeWrapper(void **ptr);


#endif //HELPER2_H
