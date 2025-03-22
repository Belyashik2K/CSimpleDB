#include "comparison.h"

#include <stdlib.h>
#include <string.h>

#include "../../../../utils/mem_profiler/helper.h"

const char *comparisonOptionStrings[] = {
    "/not_in/",
    "/in/",
    "==",
    "!=",
    "<=",
    ">=",
    "<",
    ">"
};

void freeComparison(Comparison *comparison) {
    if (!comparison) return;

    freeWrapper(comparison);
}

ComparisonOptionEnum comparisonFromString(const char *str) {
    for (int i = 0; i < COMPARE_COUNT; i++) {
        if (strcmp(str, comparisonOptionStrings[i]) == 0) {
            return (ComparisonOptionEnum) i;
        }
    }
    return COMPARE_COUNT;
}

Comparison *comparisonFactory(const char *comparisonStr) {
    const ComparisonOptionEnum comparisonEnum = comparisonFromString(comparisonStr);
    if (comparisonEnum == COMPARE_COUNT) return NULL;

    Comparison *comparison = (Comparison *) mallocWrapper(sizeof(Comparison));
    if (!comparison) return NULL;

    comparison->cOperator = comparisonEnum;

    return comparison;
}
