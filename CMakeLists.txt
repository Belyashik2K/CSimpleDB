cmake_minimum_required(VERSION 3.29.6)
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
        src/database/types/query/comparison/comparison.h
        src/database/types/record/record.c
        src/database/initializer.c
        src/database/initializer.h
        src/database/helper.c
        src/database/helper.h
        src/utils/parser/file_parser.c
        src/utils/parser/file_parser.h
        src/database/types/query/action/action.h
        src/database/types/query/action/action.c
        src/utils/writer/file_writer.c
        src/utils/writer/file_writer.h
        src/database/types/query/query.c
        src/database/types/query/query.h
        src/database/types/query/condition/condition.c
        src/database/types/query/condition/condition.h
        src/database/types/query/comparison/comparison.c
)
