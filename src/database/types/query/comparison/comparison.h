#ifndef COMPARE_H
#define COMPARE_H

typedef enum {
    NOT_IN,
    IN,
    EQUAL,
    NOT_EQUAL,
    LESS_OR_EQUAL,
    GREATER_OR_EQUAL,
    LESS,
    GREATER,
    COMPARE_COUNT
} ComparisonOptionEnum;

extern const char *comparisonOptionStrings[];

typedef struct {
    ComparisonOptionEnum cOperator;
} Comparison;

Comparison *comparisonFactory(const char *comparisonStr);

#endif
