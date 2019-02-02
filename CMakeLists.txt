set(SOURCE main.c sensor.c lcd1602.c main.h sensor.h lcd1602.h)
set(CMAKE_BUILD_TYPE Debug)
add_executable(DS18B20Reader ${SOURCE})