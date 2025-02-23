#include <stdio.h>
#include "../include/time.h"
#include "../include/date.h"
#include "../include/weather.h"
#include "../include/string.h"

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

    FILE *file = fopen("E:/AAP_SPBSTU/SimpleDB/test.txt", "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        CustomString *s = stringFactory(buffer);
        if (s != NULL) {
            printf("String: %s\n", s->value);
        } else {
            printf("String is invalid\n");
        }
    }

    return 0;
}
