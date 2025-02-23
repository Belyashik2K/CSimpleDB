cmake_minimum_required(VERSION 3.30)
project(SimpleDB C)

set(CMAKE_C_STANDARD 11)

add_executable(
        SimpleDB
        src/main.c
        src/database/types/date/date.c
        src/database/types/time/time.c
        src/database/types/weather/weather.c
        src/database/types/date/date.h
        src/database/types/time/time.h
        src/database/types/weather/weather.h
        src/database/types/record/record.h
        src/database/types/int/int.h
        src/database/types/string/string.h
        src/database/types/int/int.c
        src/database/types/string/string.c
        src/database/types/comparison/comparison.h
        src/database/types/record/record.c
        src/database/initializer.c
        src/database/initializer.h
        src/database/types/helper.c
        src/database/types/helper.h
)
