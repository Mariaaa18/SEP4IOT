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


#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"


#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
// extract queue
extern QueueHandle_t xQueue_DownLink;
extern EventGroupHandle_t _myEventGroupSender;
int co2Opt = 1000;
int humOpt = 300;
int tempOpt = 200;


struct sensors_data* downlinkData;


void retrieveQueueData()
{
    xQueueReceive(xQueue_DownLink, &downlinkData, portMAX_DELAY);
    tempOpt = downlinkData->temperature;
    co2Opt = downlinkData -> co2;
    humOpt = downlinkData -> humidity;
  

} 

void eventGroupTrigger(){

    printf("before the event group------\n");
	xEventGroupWaitBits(
		_myEventGroupSender,
		BIT_0 | BIT_1 | BIT_2,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	vTaskDelay(40);

    printf("CR: trigger by eg\n");
    
}



void actOnTemperature(){
    //Act
   

   
    if(tempOpt+50 > getTemperature()){
        //call on actuatorT class here
        printf("AA.temp too hih max: %d and current: %d\n",tempOpt,getTemperature());
        setServoHigh();
    }else if( tempOpt-50 < getTemperature()){
        //Call if colder
        printf("AA. temp too low max: %d and current: %d\n",tempOpt,getTemperature());
        setServoLow();
    } 
   
   
}

void actOnHumidity(){
    //Act
   /*  max = optimalValue->humidity + 50;
    min = optimalValue->humidity - 50;
    if(currentValue->humidity > max ){
        printf("humidity is too high\n");
        //call on actuatorH class here
    }else if(currentValue->humidity < min){
        //call if less
        printf("humidity is too low \n");
    } */
}

void actOnCo2(){
  /*   //Act
    max = optimalValue->co2 + 200;
    min = optimalValue->co2 - 200;
    if(currentValue->co2 > max ){
        printf("co 2 is too high \n");
        //call on actuatorC class here
    }else if(currentValue->co2 < min){
        printf("co 2 is too low \n");
        //call if less
    } */
}

void runRetriever(){



    eventGroupTrigger();
    retrieveQueueData();
    vTaskDelay(100);
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
       // Check if a message is available in the queue
        if (uxQueueMessagesWaiting(xQueue_DownLink) > 0) {
            
            xQueueReceive(xQueue_DownLink, &downlinkData, portMAX_DELAY);
            // Process the message
            tempOpt = downlinkData->temperature;
            co2Opt = downlinkData -> co2;
            humOpt = downlinkData -> humidity;
            printf("Received message from the queue\n");
            actOnTemperature();
        }
        
        // Check if the event bit is set in the event group
        if (xEventGroupWaitBits(
            _myEventGroupSender,
            BIT_0 | BIT_1 | BIT_2,
            pdTRUE,
            pdTRUE,
            portMAX_DELAY)) {
                xEventGroupClearBits(_myEventGroupSender,BIT_0);
                xEventGroupClearBits(_myEventGroupSender,BIT_1);
                xEventGroupClearBits(_myEventGroupSender,BIT_2);
            // Clear the event bit
          
            // Process the event
            actOnTemperature();
            printf("Event triggered\n");
        }
	
        
	}
}

void controllerReceiveTask(){
	xTaskCreate(
		setRetriever, "setRetriever", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    initServo();
}