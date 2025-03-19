#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../string/string.h"
#include "../../../utils/mem_profiler/helper.h"

void freeString(CustomString *customString) {
    if (!customString) return;

    freeWrapper(customString->field);
    freeWrapper(customString->value);
    freeWrapper(customString);
}

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
    char *buffer = (char *) malloc(bufSize);
    if (!buffer) return NULL;
    snprintf(buffer, bufSize, "%s=\"%s\"", self->field, self->value);
    return buffer;
}

int equalString(CustomString *self, char *other) {
    CustomString *otherString = stringFactory(other, self->field);
    if (!otherString) return 0;

    int result = strcmp(self->value, otherString->value) == 0;
    freeString(otherString);

    return result;
}

int notEqualString(CustomString *self, char *other) {
    return !equalString(self, other);
}

int lessString(CustomString *self, char *other) {
    CustomString *otherString = stringFactory(other, self->field);
    if (!otherString) return 0;

    int result = strcmp(self->value, otherString->value) < 0;
    freeString(otherString);

    return result;
}

int greaterString(CustomString *self, char *other) {
    CustomString *otherString = stringFactory(other, self->field);
    if (!otherString) {
        return 0;
    }

    int result = strcmp(self->value, otherString->value) > 0;
    freeString(otherString);

    return result;
}

int lessOrEqualString(CustomString *self, char *other) {
    return lessString(self, other) || equalString(self, other);
}

int greaterOrEqualString(CustomString *self, char *other) {
    return greaterString(self, other) || equalString(self, other);
}

int compareStrings(CustomString *self, char *other, ComparisonOptionEnum option) {
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

int updateString(CustomString *self, char *newValue) {
    CustomString *newString = stringFactory(newValue, self->field);
    if (!newString) return 0;

    freeWrapper(self->value);
    self->value = strdupWrapper(newString->value);
    freeString(newString);
    return 1;
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

    CustomString *customString = (CustomString *) mallocWrapper(sizeof(CustomString));
    if (customString == NULL) return NULL;

    customString->value = (char *) mallocWrapper(strlen(string) + 1);
    if (customString->value == NULL) {
        freeString(customString);
        return NULL;
    }


    strcpy(customString->value, string + 1);
    customString->value[strlen(customString->value) - 1] = '\0';

    customString->field = strdupWrapper(field);
    if (customString->field == NULL) {
        freeString(customString);
        return NULL;
    }

    customString->toString = stringToString;
    customString->compare = compareStrings;
    customString->update = updateString;

    return customString;
}
