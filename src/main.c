#include <stdio.h>
#include "../src/database/record/record.h"

int main(void) {

    char *insertCommands[] = {
        "insert   sundown='19:07:03',geo_pos=\"Penkin\",level=57,mea_date='20.03.2016',weather='snow',sunrise='19:07:42',geo_id=0016",
        "insert   level=004,geo_id=-10,mea_date='10.04.2016',geo_pos=\"Bobrov\",sunrise='19:07:42',weather='snow',sundown='18:41:50'",
        "insert   weather='snow',level=0,mea_date='20.03.2016',sundown='18:42:00',geo_id=004,geo_pos=\"Golosov\",sunrise='19:05:41'",
        "insert     sunrise='18:41:50',mea_date='29.12.2016',geo_id=-19,sundown='18:40:52',weather='snow',geo_pos=\"Bobrov\",level=57",
        "insert     level=18,mea_date='15.06.2016',geo_pos=\"Vodkin\",sunrise='18:40:51',geo_id=41,sundown='19:07:55',weather='cloudy'",
        "insert     sundown='19:07:55',level=041,sunrise='19:07:03',weather='snow',geo_pos=\"Kuzin\",geo_id=41,mea_date='28.02.2017'",
        "insert    sundown='18:42:00',mea_date='01.02.2017',weather='fair',geo_pos=\"Penkin\",level=0,geo_id=18,sunrise='19:07:55'",
        "insert   weather='cloudy',geo_id=-10,sunrise='23:59:59',geo_pos=\"Kuzin\",sundown='18:41:50',mea_date='01.03.2017',level=004",
        "insert mea_date='01.03.2017',sundown='18:42:00',weather='rain',geo_pos=\"Bobrov\",level=57,sunrise='18:40:51',geo_id=551",
        "insert   weather='rain',sunrise='19:05:41',mea_date='10.04.2016',level=57,geo_id=0016,sundown='19:07:42',geo_pos=\"Albin\"",
        "insert  geo_pos=\"Bobrovsky\",mea_date='20.12.2016',level=0,sunrise='19:07:42',weather='cloudy',sundown='18:40:52',geo_id=40",
        "insert sunrise='19:07:55',weather='cloudy',level=57,sundown='23:59:59',mea_date='20.12.2016',geo_pos=\"Suslikov\",geo_id=41",
        "insert  mea_date='15.06.2016',sundown='19:07:55',weather='fair',geo_pos=\"Kuzin\",sunrise='18:42:00',level=551,geo_id=004",
        "insert sundown='19:05:41',mea_date='01.03.2017',geo_id=0,weather='fair',sunrise='19:07:42',level=41,geo_pos=\"Golosov\"",
        "insert    sunrise='19:05:41',geo_id=15,sundown='18:40:52',mea_date='01.02.2017',geo_pos=\"Golosov\",level=0015,weather='rain'"
    };


    for (int i = 0; i < sizeof(insertCommands) / sizeof(insertCommands[0]); i++) {
        Record *r = recordFactory(insertCommands[i]);
        if (r != NULL) {
            printf("Record: %d, %s, '%02d.%02d.%02d', %d, '%02d:%02d:%02d', %s, '%02d:%02d:%02d'\n",
                   r->geo_id.value,
                   r->geo_pos.value,
                   r->mea_date.day,
                   r->mea_date.month,
                   r->mea_date.year,
                   r->level.value,
                   r->sunrise.hour,
                   r->sunrise.minute,
                   r->sunrise.second,
                   weather_to_string(r->weather),
                   r->sundown.hour,
                   r->sundown.minute,
                   r->sundown.second
            );
        } else {
            printf("Record is invalid\n");
        }
    }

    return 0;
}
