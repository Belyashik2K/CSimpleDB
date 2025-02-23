#include <stdio.h>
#include "../include/time.h"
#include "../include/date.h"
#include "../include/weather.h"

int main(void) {
    const Time *t = timeFactory("'12:34:56'");
    if (t != NULL) {
        printf("Time: %d:%d:%d\n", t->hour, t->minute, t->second);
    }

    const Date *d = dateFactory("'15.01.2025'");
    if (d != NULL) {
        printf("Date: %d.%d.%d\n", d->day, d->month, d->year);
    }

    const char *w_str = weather_to_string(FAIR);
    printf("Weather: %s\n", w_str);

    return 0;
}
