#include <string.h>
#include "../include/weather.h"

const char *weather_strings[] = {
    "'fair'",
    "'rain'",
    "'cloudy'",
    "'snow'"
};

const char *weather_to_string(const Weather weather) {
    if (weather >= WEATHER_COUNT) {
        return "unknown";
    }
    return weather_strings[weather];
}

Weather weather_from_string(const char *str) {
    for (int i = 0; i < WEATHER_COUNT; i++) {
        if (strcmp(str, weather_strings[i]) == 0) {
            return (Weather) i;
        }
    }
    return WEATHER_COUNT;
}
