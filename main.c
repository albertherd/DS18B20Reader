#include <stdio.h> 
#include <time.h>
#include <signal.h>

#include "main.h"
#include "sensor.h"
#include "lcd1602.h"

volatile sig_atomic_t sigintFlag = 0;

void onSigInt(int signum){
    sigintFlag = 1;
}

void InitializeLCD()
{
    int rc;
	rc = lcd1602Init(1, LCDADDRESS);
	if (rc)
	{
		printf("Initialization failed; aborting...\n");
		return;
	}
}

int main(int argc, char **argv)
{ 
    signal(SIGINT, onSigInt);
    InitializeLCD();

    char **sensorNames;
    int sensorNamesCount;
    if(argc > 1)
    {
        sensorNames = argv + 1;
        sensorNamesCount = argc - 1;
    }    
    else
    {
        sensorNames = NULL;
        sensorNamesCount = 0;
    }    

    SensorList *sensorList = GetSensors(sensorNames, sensorNamesCount);
    if(sensorList->SensorCount == 0)
    {
        printf("No sensors found - exiting.\n");
        return 0;
    }
    printf("Attached Sensors: %d\n", sensorList->SensorCount);

    ReadTemperatureLoop(sensorList);        
    Cleanup(sensorList);   
}


void Cleanup(SensorList *sensorList)
{
    printf("Exiting...\n");
    FreeSensors(sensorList);
    lcd1602Shutdown();
}

void ReadTemperatureLoop(SensorList *sensorList)
{
    while(!sigintFlag)
    {
        for(int i = 0; i < sensorList->SensorCount; i++)
        {
            float temperature = ReadTemperature(sensorList->Sensors[i]);
            PrintTemperatueToLCD1602(sensorList->Sensors[i], i % LCD1602LINES, temperature);
            LogTemperature(sensorList->Sensors[i], temperature);
        }       
    }
}

void LogTemperature(Sensor *sensor, float temperature)
{
    time_t currentTime;
    time(&currentTime);

    char dateTimeStringBuffer[32];
    strftime(dateTimeStringBuffer, 32, "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

    printf("%s - %s - %.2fC\n", dateTimeStringBuffer, sensor->SensorName, temperature);
}

void PrintTemperatueToLCD1602(Sensor *sensor, int lineToPrintDataOn, float temperature)
{
    char temperatureString[LCD1602CHARACTERS + 1];
    snprintf(temperatureString, LCD1602CHARACTERS + 1, "%s : %.2fC", sensor->SensorName, temperature);

    lcd1602SetCursor(0, lineToPrintDataOn);
	lcd1602WriteString(temperatureString);
}