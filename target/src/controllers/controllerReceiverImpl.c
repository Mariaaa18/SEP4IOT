#include <stdio.h>
// #include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerReceiver.h"
#include "dataShared.h"
#include "../models/actuator.h"

/*
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
*/

// extract queue
extern QueueHandle_t xQueue_DownLink;

struct sensors_data* currentValue;
struct sensors_data* optimalValue;
struct sensors_data* downlinkData;
static uint16_t max = 0;
static uint16_t min = 0;

void retrieveQueueData()
{
    xQueueReceive(xQueue_DownLink, &downlinkData, portMAX_DELAY);
    printf("\n ReceivedData from Downlink Queue \n");
    vTaskDelay(10);
    optimalValue->co2 = downlinkData->co2;
    vTaskDelay(10);
    printf("FromQueueDownlink: %d \n", downlinkData->co2);
    optimalValue->temperature = downlinkData->temperature;
    optimalValue->humidity = downlinkData->humidity;
}   

void setCurrentValue(){
    currentValue = getSensorData();
    vTaskDelay(10);
    printf("FromCurrentValue: %d \n", currentValue->co2);
}

void actOnTemperature(){
    //Act
   
   max = optimalValue->temperature + 5;
   min = optimalValue->temperature - 5;
    if(currentValue->temperature > max){
        //call on actuatorT class here
       // printf("AA.temp too hih\n");
         setServoHigh();
    }else if( currentValue->temperature < min){
        //Call if colder
       // printf("AA. temp too low\n");
        setServoLow();
    }
}

void actOnHumidity(){
    //Act
    max = optimalValue->humidity + 5;
    min = optimalValue->humidity - 5;
    if(currentValue->humidity > max ){
        printf("humidity is too high\n");
        //call on actuatorH class here
    }else if(currentValue->humidity < min){
        //call if less
        printf("humidity is too low \n");

    }
}

void actOnCo2(){
    //Act
    max = optimalValue->co2 + 5;
    min = optimalValue->co2 - 5;
    if(currentValue->co2 > max ){
        printf("co 2 is too high \n");
        //call on actuatorC class here
    }else if(currentValue->co2 < min){
        printf("co 2 is too low \n");
        //call if less
    }
}

void runRetriever(){
   
   
    retrieveQueueData();
    
    setCurrentValue();
   
    //actOnTemperature();
    //actOnHumidity();
    //actOnCo2();

    
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

    initServo();
}