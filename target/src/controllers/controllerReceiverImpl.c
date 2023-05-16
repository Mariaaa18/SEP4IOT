#include <stdio.h>
// #include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerReceiver.h"
#include "dataShared.h"

/*
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
*/

// extract queue
extern QueueHandle_t xQueueDownlink;

struct sensors_data* currentValue;
struct sensors_data* optimalValue;
struct sensors_data downlinkData;
static uint16_t max = 0;
static uint16_t min = 0;

void retrieveQueueData()
{
    xQueueReceive(xQueueDownlink, &downlinkData, portMAX_DELAY);
    printf("\n ReceivedData from Downlink Queue \n");
    optimalValue->co2 = downlinkData.co2;
    optimalValue->temperature = downlinkData.temperature;
    optimalValue->humidity = downlinkData.humidity;
}   

void setCurrentValue(){
    currentValue = getSensorData();
}

void actOnTemperature(){
    //Act
    max = optimalValue->temperature + 5;
    min = optimalValue->temperature - 5;
    if(currentValue->temperature > max){
        //call on actuatorT class here
    }else if( currentValue->temperature < min){
        //Call if colder
    }
}

void actOnHumidity(){
    //Act
    max = optimalValue->humidity + 5;
    min = optimalValue->humidity - 5;
    if(currentValue->humidity > max ){
        //call on actuatorH class here
    }else if(currentValue->humidity < min){
        //call if less
    }
}

void actOnCo2(){
    //Act
    max = optimalValue->co2 + 5;
    min = optimalValue->co2 - 5;
    if(currentValue->co2 > max ){
        //call on actuatorC class here
    }else if(currentValue->co2 < min){
        //call if less
    }
}

void runRetriever(){
    retrieveQueueData();
    setCurrentValue();
    actOnTemperature();
    actOnHumidity();
    actOnCo2();
}

void setRetriever( void *p){
    (void)p;
	for (;;)
	{
		printf("RetrieveData---\n");
		runRetriever();
	}
}

void controllerReceiveTask(){
	xTaskCreate(
		setRetriever, "setRetriever", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}