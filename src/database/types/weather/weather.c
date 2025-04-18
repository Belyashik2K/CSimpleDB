#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../query/query.h"
#include "../weather/weather.h"
#include "../../../utils/mem_profiler/helper.h"


const char *weather_strings[] = {
    "fair",
    "rain",
    "cloudy",
    "snow"
};

void freeWeather(Weather *weather) {
    if (!weather) return;
    freeWrapper(weather->field);
    freeWrapper(weather);
}

char *weatherToString(Weather *self) {
    const int bufSize = strlen(self->field) + strlen(weather_strings[self->value]) + 50;
    char *buffer = (char *) mallocWrapper(bufSize);
    if (!buffer) return NULL;
    sprintf(buffer, "%s='%s'", self->field, weather_strings[self->value]);
    return buffer;
}

int isValueList(const char *input) {
    return strchr(input, '[') != NULL && strchr(input, ']') != NULL;
}

int validateValuesInList(const char *input) {
    char *tempPtr = strdupWrapper(input);
    char *temp = tempPtr;
    if (!temp) {
        return 0;
    }

    if (strcmp(temp, "[]") == 0) {
        freeWrapper(tempPtr);
        return 1;
    }

    char *start = strchr(temp, '[');
    char *end = strrchr(temp, ']');

    if (!start || !end || start > end) {
        freeWrapper(tempPtr);
        return 0;
    }

    *end = '\0';
    start++;

    char *inside = trimWhitespace(start);
    if (strlen(inside) == 0) {
        freeWrapper(tempPtr);
        return 0;
    }

    char *token = strtok(inside, ",");

    while (token != NULL) {
        token = trimWhitespace(token);

        int len = strlen(token);
        if (len >= 2 && token[0] == '\'' && token[len - 1] == '\'') {
            token[len - 1] = '\0';
            token++;
            len -= 2;
        }

        int valid = 0;
        for (int i = 0; i < WEATHER_COUNT; i++) {
            if (strcmp(token, weather_strings[i]) == 0) {
                valid = 1;
                break;
            }
        }

        if (!valid) {
            freeWrapper(tempPtr);
            return 0;
        }

        token = strtok(NULL, ",");
    }

    freeWrapper(tempPtr);
    return 1;
}

int findInList(Weather *self, char *other) {
    char *tempPtr = strdupWrapper(other);
    char *temp = tempPtr;

    if (!temp) {
        return 0;
    }

    char *start = strchr(temp, '[');
    char *end = strrchr(temp, ']');

    if (!start || !end || start > end) {
        freeWrapper(tempPtr);
        return 0;
    }

    *end = '\0';
    start++;

    char *inside = trimWhitespace(start);
    if (strlen(inside) == 0) {
        freeWrapper(tempPtr);
        return 0;
    }

    char *token = strtok(inside, ",");

    while (token != NULL) {
        token = trimWhitespace(token);

        int len = strlen(token);
        if (len >= 2 && token[0] == '\'' && token[len - 1] == '\'') {
            token[len - 1] = '\0';
            token++;
            len -= 2;
        }

        Weather *tempWeather = weatherFactory(token, self->field);
        if (!tempWeather) {
            freeWrapper(tempPtr);
            return -1;
        }
        freeWeather(tempWeather);

        if (strcmp(token, weather_strings[self->value]) == 0) {
            freeWrapper(tempPtr);
            return 1;
        }

        token = strtok(NULL, ",");
    }

    freeWrapper(tempPtr);
    return 0;
}

int inWeatherList(Weather *self, char *other) {
    if (!isValueList(other)) return 0;
    return findInList(self, other) == 1 ? 1 : 0;
}

int notInWeatherList(Weather *self, char *other) {
    return findInList(self, other) == 0 ? 1 : 0;
}

int equalWeather(Weather *self, char *other) {
    Weather *otherWeather = weatherFactory(other, self->field);
    if (!otherWeather) return 0;

    const int result = self->value == otherWeather->value;
    freeWeather(otherWeather);
    return result;
}

int notEqualWeather(Weather *self, char *other) {
    return !equalWeather(self, other);
}

int compareWeathers(Weather *self, char *other, ComparisonOptionEnum option) {
    switch (option) {
        case EQUAL:
            return equalWeather(self, other);
        case NOT_EQUAL:
            return notEqualWeather(self, other);
        case IN:
            return inWeatherList(self, other);
        case NOT_IN:
            return notInWeatherList(self, other);
        default:
            return 0;
    }
}

int updateWeatherField(Weather *self, char *newValue) {
    Weather *newWeather = weatherFactory(newValue, self->field);
    if (!newWeather) return 0;

    self->value = newWeather->value;
    freeWeather(newWeather);
    return 1;
}

Weather *weatherFactory(const char *weatherString, const char *field) {
    int valid = 0;

    if (isValueList(weatherString)) {
        valid = validateValuesInList(weatherString);
    }

    int i = 0;

    char *withoutQuotesPtr = strdupWrapper(weatherString);
    char *withoutQuotes = withoutQuotesPtr;
    if (!withoutQuotes) return NULL;

    if (withoutQuotes[0] == '\'' && withoutQuotes[strlen(withoutQuotes) - 1] == '\'') {
        withoutQuotes++;
        withoutQuotes[strlen(withoutQuotes) - 1] = '\0';
    }

    for (; i < WEATHER_COUNT; i++) {
        if (strcmp(withoutQuotes, weather_strings[i]) == 0 || valid) {
            valid = 1;
            break;
        }
    }

    freeWrapper(withoutQuotesPtr);
    if (!valid) return NULL;

    Weather *weather = (Weather *) mallocWrapper(sizeof(Weather));
    if (!weather) return NULL;

    weather->value = (WeatherEnum) i;
    weather->field = strdupWrapper(field);

    weather->toString = weatherToString;
    weather->compare = compareWeathers;
    weather->update = updateWeatherField;

    return weather;
}
