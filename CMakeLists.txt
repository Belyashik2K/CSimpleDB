cmake_minimum_required(VERSION 3.30)
project(SimpleDB C)

set(CMAKE_C_STANDARD 11)

add_executable(
        SimpleDB
        src/main.c
        src/date.c
        src/time.c
        src/weather.c
        include/date.h
        include/time.h
        include/weather.h
        include/record.h
        include/int.h
        include/string.h
        src/int.c
        src/string.c
)
