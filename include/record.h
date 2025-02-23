#ifndef RECORD_H
#define RECORD_H

#include "../include/time.h"
#include "../include/weather.h"
#include "../include/date.h"
#include "../include/int.h"
#include "../include/string.h"

typedef struct {
    CustomInt geo_id;
    CustomString geo_pos;
    Date mea_date;
    Time sunrise;
    CustomInt level;
    Weather weather;
    Time sundown;
} Record;

#endif
