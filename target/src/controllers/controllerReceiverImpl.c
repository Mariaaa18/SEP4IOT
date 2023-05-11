#include <stdio.h>
// #include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerReceiver.h"
#include "shared/sharedData.h"

/*
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
*/

// extract queue
extern QueueHandle_t xQueueDownlink;

extern sensors_data currentValue;
struct sensors_data* optimalValue;
struct sensors_data downlinkData;

void retrieveQueueData()
{
    xQueueReceive(xQueueDownlink, &downlinkData, portMAX_DELAY);
    optimalValue->co2 = downlinkData.co2;
    optimalValue->temperature = downlinkData.temperature;
    optimalValue->humidity = downlinkData.humidity;
}   

void setCurrentValue(){
    //Get value from Mutex
}

void actOnTemperature(){
    //Act
}

void actOnHumidity(){
    //Act
}

void actOnCo2(){
    //Act
}

void runRetriever(){
    retrieveQueueData();
    setCurrentValue();
    
}

void setRetriever( void *p){
    (void)p;
	for (;;)
	{
		printf("RetrieveData---\n");
		run();
	}
}

void controllerReceiveTask(){
	xTaskCreate(
		setRetriever, "setRetriever", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);
}