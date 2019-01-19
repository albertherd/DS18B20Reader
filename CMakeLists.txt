set(SOURCE main.c sensor.c main.h sensor.h)
set(CMAKE_BUILD_TYPE Debug)
add_executable(DS18B20Reader ${SOURCE})