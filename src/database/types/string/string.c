#include <stdlib.h>
#include <string.h>

#include "../string/string.h"

#include <stdio.h>


int checkForScreening(const char previous, const int index) {
    return index < 0 ? 0 : previous == (char) 92 ? 1 : 0;
}

int checkDoubleQuotesScreening(const char *string, int *index) {
    if (string[*index] != '"') {
        return 1;
    }
    if (checkForScreening(string[*index - 1], *index - 1)) {
        *index -= 1;
        return 1;
    }
    return 0;
}

int checkBackslashScreening(const char *string, int *index) {
    if (string[*index] != (char) 92 || *index - 1 == 0) {
        return 1;
    }
    if (string[*index - 1] == (char) 92) {
        *index -= 1;
        return 1;
    }
    return 0;
}

char *stringToString(CustomString *self) {
    const int bufSize = strlen(self->field) + strlen(self->value) + 50;
    char *buffer = malloc(bufSize);
    if (!buffer) return NULL;
    snprintf(buffer, bufSize, "%s=%s", self->field, self->value);
    return buffer;
}

int equalString(const CustomString *self, char *other) {
    CustomString *otherString = stringFactory(other, self->field);
    if (!otherString) return 0;

    int result = strcmp(self->value, otherString->value) == 0;
    free(otherString);

    return result;
}

int notEqualString(const CustomString *self, char *other) {
    return !equalString(self, other);
}

int lessString(const CustomString *self, char *other) {
    CustomString *otherString = stringFactory(other, self->field);
    if (!otherString) return 0;

    int result = strcmp(self->value, otherString->value) < 0;
    free(otherString);

    return result;
}

int greaterString(const CustomString *self, char *other) {
    CustomString *otherString = stringFactory(other, self->field);
    if (!otherString) return 0;

    int result = strcmp(self->value, otherString->value) > 0;
    free(otherString);

    return result;
}

int lessOrEqualString(const CustomString *self, char *other) {
    return lessString(self, other) || equalString(self, other);
}

int greaterOrEqualString(const CustomString *self, char *other) {
    return greaterString(self, other) || equalString(self, other);
}

int compareStrings(const CustomString *self, char *other, ComparisonOptionEnum option) {
    switch (option) {
        case EQUAL:
            return equalString(self, other);
        case NOT_EQUAL:
            return notEqualString(self, other);
        case LESS:
            return lessString(self, other);
        case GREATER:
            return greaterString(self, other);
        case LESS_OR_EQUAL:
            return lessOrEqualString(self, other);
        case GREATER_OR_EQUAL:
            return greaterOrEqualString(self, other);
        default:
            return 0;
    }
}

CustomString *stringFactory(char *string, const char *field) {
    if (string[0] != '"' || string[strlen(string) - 1] != '"') {
        return NULL;
    }

    for (int i = strlen(string) - 2; i > 0; i--) {
        if (!checkBackslashScreening(string, &i)) {
            return NULL;
        }
        if (!checkDoubleQuotesScreening(string, &i)) {
            return NULL;
        }
    }

    CustomString *customString = malloc(sizeof(CustomString));
    if (customString == NULL) return NULL;

    customString->value = malloc(strlen(string) + 1);
    if (customString->value == NULL) {
        free(customString);
        return NULL;
    }

    strcpy(customString->value, string);

    customString->field = strdup(field);
    if (customString->field == NULL) {
        free(customString->value);
        free(customString);
        return NULL;
    }
    customString->toString = stringToString;
    customString->compare = compareStrings;

    return customString;
}
