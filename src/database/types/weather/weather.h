#ifndef WEATHER_H
#define WEATHER_H

#include "../../../database/types/query/comparison/comparison.h"

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
    int (*compare)(struct Weather *self, char *other, ComparisonOptionEnum option);
} Weather;

Weather *weatherFactory(const char *weatherString, const char *field);

#endif
