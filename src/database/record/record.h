#ifndef RECORD_H
#define RECORD_H

#include "../types/int/int.h"
#include "../types/string/string.h"
#include "../types/date/date.h"
#include "../types/time/time.h"
#include "../types/weather/weather.h"

typedef struct {
    CustomInt geo_id;
    CustomString geo_pos;
    Date mea_date;
    CustomInt level;
    Time sunrise;
    Weather weather;
    Time sundown;
} Record;

Record *recordFactory(char *recordString);

#endif
