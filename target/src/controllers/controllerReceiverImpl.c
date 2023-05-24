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
#include "../models/mystruct.h"

/*
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
*/

// extract queue
extern QueueHandle_t xQueue_DownLink;
extern EventGroupHandle_t _myEventGroupSender;

struct sensors_data* currentValue;
struct MyData dataRecievd;

int co2Opt = 1000;
int humOpt = 300;
int tempOpt = 200;

int curTemp = 0;
int curHum = 0;
int curCo2 = 0;

#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)


void retrieveQueueData()
{
    xQueueReceive(xQueue_DownLink, &dataRecievd, portMAX_DELAY);
    printf("data recieved temp:%d\n",dataRecievd.temp);
    currentValue= getSensorData();
    printf("dat from datasherd%d:\n", currentValue->temperature);
}   

void setCurrentValue(){


    currentValue= getSensorData();
    curTemp = currentValue->temperature;
    curHum = currentValue->humidity;
    curCo2 = currentValue->co2;
    //printf("temp in reciever :%d",currentValue->temperature);
}

void setOptimalValues(){
    co2Opt = dataRecievd.co2;
    humOpt = dataRecievd.hum;
    tempOpt = dataRecievd.temp;
}

void actOnTemperature(){
    //Act
    if(tempOpt+50 > curTemp){
        //call on actuatorT class here
        printf("AA.temp too hih max: %d and current: %d\n",tempOpt+50,curTemp);
        setServoHigh();
    }else if( tempOpt-50 < curTemp){
        //Call if colder
        printf("AA. temp too low max: %d and current: %d\n",tempOpt-50,curTemp);
        setServoLow();
    } 

    
   
   
}

void actOnHumidity(){
    //Act
   
}

void actOnCo2(){
    //Act
  
}

void runRetriever(){
   
   
    retrieveQueueData();
    //setCurrentValue();
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
            
            xQueueReceive(xQueue_DownLink, &dataRecievd, portMAX_DELAY);
            setOptimalValues();


            
            // Process the message
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
            setCurrentValue();
            vTaskDelay(100);
           actOnTemperature();
           vTaskDelay(100);
            actOnHumidity();
           vTaskDelay(100);
           actOnCo2();
            
            
             
        }
        
	}
}

void controllerReceiveTask(){
	xTaskCreate(
		setRetriever, "setRetriever", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    initServo();
}