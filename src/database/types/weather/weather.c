#include <string.h>

#include "../weather/weather.h"

#include <stdio.h>
#include <stdlib.h>

const char *weather_strings[] = {
    "'fair'",
    "'rain'",
    "'cloudy'",
    "'snow'"
};

char *weatherToString(Weather *self) {
    const int bufSize = strlen(self->field) + strlen(weather_strings[self->value]) + 50;
    char *buffer = malloc(bufSize);
    if (!buffer) return NULL;
    snprintf(buffer, bufSize, "%s=%s", self->field, weather_strings[self->value]);
    return buffer;
}

Weather *weatherFactory(const char *weatherString, const char *field) {
    for (int i = 0; i < WEATHER_COUNT; i++) {
        if (strcmp(weatherString, weather_strings[i]) == 0) {
            Weather *weather = malloc(sizeof(Weather));
            if (!weather) return NULL;

            weather->value = (WeatherEnum) i;
            weather->field = strdup(field);
            if (!weather->field) {
                free(weather);
                return NULL;
            }

            weather->toString = weatherToString;
            weather->compare = NULL;

            return weather;
        }
    }
    return NULL;
}
