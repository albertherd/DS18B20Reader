#ifndef _main_h
#define _main_h 

#include "sensor.h"

void Cleanup(SensorList *sensorList);
void ReadTemperatureLoop(SensorList *sensorList);

#endif /* _main_h */