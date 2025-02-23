#ifndef WEATHER_H
#define WEATHER_H

typedef enum {
    FAIR,
    RAIN,
    CLOUDY,
    SNOW,
    WEATHER_COUNT
} Weather;

extern const char *weather_strings[];

const char *weather_to_string(Weather weather);
Weather weather_from_string(const char *str);

#endif
