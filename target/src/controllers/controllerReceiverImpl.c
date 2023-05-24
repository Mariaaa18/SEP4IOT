#include <stdio.h>
// #include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <display_7seg.h>
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
    //printf("data recieved temp:%d\n",dataRecievd.temp);
    currentValue= getSensorData();
    // printf("dat from datasherd%d:\n", currentValue->temperature);
}   

void setCurrentValue(){


    currentValue= getSensorData();
    curTemp = currentValue->temperature;
    curHum = currentValue->humidity*10;
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
        printf("AA.temp too low min: %d and current: %d\n-- increse temp by:%d \n",tempOpt+50,curTemp, (tempOpt+50)-curTemp);
        setServoLow();
        
    }else if( tempOpt-50 < curTemp){
        //Call if colder
        printf("AA. temp too high max: %d and current: %d\n-- decrease temp by:%d\n",tempOpt-50,curTemp,curTemp-(tempOpt-50));
      
        setServoHigh();
       
    } 

}

void actOnHumidity(){
    //Act
      if(humOpt+50 > curHum){
        //call on actuatorT class here
       printf("AA.hum too low min: %d and current: %d\n-- increse hum by:%d \n",humOpt+50,curHum, (humOpt+50)-curHum);
       
    }else if( humOpt-50 < curHum){
        //Call if colder
        printf("AA. hum too high max: %d and current: %d\n-- decrease hum by:%d\n",humOpt-50,curHum,curHum-(humOpt-50));
      
       
    } 
}

void actOnCo2(){
    //Act
    
    if(co2Opt > curCo2){
        
        //call on actuatorT class here
       printf("AA.co2 too low min: %d and current: %d\n",co2Opt,curCo2 );
    
    } else if( co2Opt < curCo2){
        //Call if colder
       printf("AA.co2 too high max: %d and current: %d\n",co2Opt,curCo2);
      //display_7seg_displayHex("C022");
    }   
   //display_7seg_displayHex("C022");
   
  
   
    
    
}



void setRetriever( void *p){
    (void)p;
	while(1)
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