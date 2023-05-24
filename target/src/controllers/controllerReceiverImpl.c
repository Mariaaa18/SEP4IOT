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

int curTemp = 0;
int curHum = 0;
int curCo2 = 0;


struct sensors_data* downlinkData;

void getTempFromSensor(){
    curTemp = getTemperature();
  
}
void getHumFromSensor(){
    curHum = getHumidity();
  
}







void actOnTemperature(){
    //Act
   
 getTempFromSensor();
   
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
   getHumFromSensor();
    if(humOpt+50 > curHum ){
        printf("humidity is too high\n");
        //call on actuatorH class here
    }else if(humOpt-50 < curHum ){
        //call if less
        printf("humidity is too low \n");
    } 
}

void actOnCo2(){
 curCo2 = getCoTwo();
    if(co2Opt+200 < curCo2){
        printf("co 2 is too high \n");
        //call on actuatorC class here
    }else if(humOpt-200 > curCo2){
        printf("co 2 is too low \n");
        //call if less
    }
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
            printf("CR:Recieved from queue temp:%d",tempOpt);
            co2Opt = downlinkData -> co2;
            humOpt = downlinkData -> humidity;
           
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