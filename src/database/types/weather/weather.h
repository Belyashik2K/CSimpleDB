#ifndef WEATHER_H
#define WEATHER_H

typedef enum {
    FAIR,
    RAIN,
    CLOUDY,
    SNOW,
    WEATHER_COUNT
} WeatherEnum;

typedef struct Weather {
    WeatherEnum value;
    char *field;

    char *(*toString)(struct Weather *self);
} Weather;

Weather *weatherFactory(const char *weatherString, const char *field);

#endif
