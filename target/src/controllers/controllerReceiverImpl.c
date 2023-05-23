#include <stdio.h>
// #include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerReceiver.h"
#include "dataShared.h"
#include "FreeRTOSVariant.h"
#include "../models/temperature.h"
#include "../models/actuator.h"

/*
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
*/

// extract queue
extern QueueHandle_t xQueue_DownLink;
extern dataShared_t dataSensors;


struct dataShared_t downlinkData;
static uint16_t max = 0;
static uint16_t min = 0;

void retrieveQueueData()
{
    xQueueReceive(xQueue_DownLink, &downlinkData, portMAX_DELAY);
}   



void actOnTemperature(){
    //Act
   
  max = dataShared_getTemperature(downlinkData) + 50;
   min = downlinkData->temperature - 50;
   
    if(dataShared_getTemperature(dataSensors)> max){
        //call on actuatorT class here
        printf("AA.temp too hih max: %d and current: %d\n",max,currentValue->temperature);
        setServoHigh();
    }else if( dataShared_getTemperature(dataSensors)< min){
        //Call if colder
        printf("AA. temp too low max: %d and current: %d\n",max,currentValue->temperature);
        setServoLow();
    } 
   
   
}

void actOnHumidity(){
    //Act
    max = downlinkData->humidity + 50;
    min = downlinkData->humidity - 50;

    if(dataShared_getHumidity(dataSensors) > max ){
        printf("humidity is too high\n");
        //call on actuatorH class here
    }else if(dataShared_getHumidity(dataSensors) < min){
        //call if less
        printf("humidity is too low \n");
    }
}

void actOnCo2(){
    //Act
    max = downlinkData->co2 + 200;
    min = downlinkData->co2 - 200;
    if(dataShared_getCo2(dataSensors)> max ){
        printf("co 2 is too high \n");
        //call on actuatorC class here
    }else if(dataShared_getCo2(dataSensors)< min){
        printf("co 2 is too low \n");
        //call if less
    }
}

void runRetriever(){
   
   
    retrieveQueueData();
    setCurrentValue();
    actOnTemperature();
    vTaskDelay(100);
    actOnHumidity();
    vTaskDelay(100);
    actOnCo2();
    
}

void setRetriever( void *p){
    (void)p;
	for (;;)
	{
		//printf("RetrieveData---\n");
     
		runRetriever();
        
	}
}

void controllerReceiveTask(){
	xTaskCreate(
		setRetriever, "setRetriever", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    initServo();
}