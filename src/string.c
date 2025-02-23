#include "../include/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkForScreening(const char previous, const int index) {
    return index < 0 ? 0 : previous == (char) 92 ? 1 : 0;
}

int checkDoubleQuotesScreening(const char *string, int* index) {
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

CustomString *stringFactory(char *string) {
    for (int i = strlen(string) - 2; i > 0 ; i--) {
        if (!checkBackslashScreening(string, &i)) {
            return NULL;
        }
        if (!checkDoubleQuotesScreening(string, &i)) {
            return NULL;
        }
    }

    CustomString *customString = malloc(sizeof(CustomString));
    if (customString == NULL) return NULL;
    customString->value = string;
    return customString;
}
