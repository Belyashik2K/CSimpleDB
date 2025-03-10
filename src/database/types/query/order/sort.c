#include "sort.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../../../utils/mem_profiler/helper.h"

const char *sortStrings[] = {
    "asc",
    "desc"
};

void freeSort(Sort *sort) {
    if (!sort) return;
    freeWrapper(sort);
}

SortEnum sortFromString(const char *str) {
    for (int i = 0; i < SORT_COUNT; i++) {
        if (strcmp(str, sortStrings[i]) == 0) {
            return (SortEnum) i;
        }
    }
    return SORT_COUNT;
}

Sort *sortFactory(char *sortString) {
    if (!sortString) return NULL;
    const SortEnum sortEnum = sortFromString(sortString);
    if (sortEnum == SORT_COUNT) return NULL;

    Sort *sort = (Sort *) mallocWrapper(sizeof(Sort));
    if (!sort) return NULL;

    sort->value = sortEnum;

    return sort;
}
