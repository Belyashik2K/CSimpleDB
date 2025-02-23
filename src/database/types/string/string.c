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

    return customString;
}
