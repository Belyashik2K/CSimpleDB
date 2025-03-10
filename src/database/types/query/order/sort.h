//
// Created by Belyashik2K on 27.02.2025.
//

#ifndef SORT_H
#define SORT_H

typedef enum {
    ASC,
    DESC,
    SORT_COUNT
} SortEnum;

typedef struct Sort {
    SortEnum value;
} Sort;

Sort *sortFactory(char *sortString);
void freeSort(Sort *sort);

#endif //SORT_H
