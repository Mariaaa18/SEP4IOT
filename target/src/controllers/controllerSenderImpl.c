#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerSender.h"

//define queue
extern QueueHandle_t xQueue2;

//struct that will keep the data to be sent to the queue
struct sensors_data data;
extern EventGroupHandle_t EventGroupHandle;
//sensor bits
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
//setData bit
#define BIT_4 (1 << 4)


/*-----------------------------------------------------------*/
void controllerSenderTask(void)
{
	//Create tasks
	xTaskCreate(
    sendDataToQueue
	,  "sendData"
	,  configMINIMAL_STACK_SIZE+200
	,  NULL
	,  1
	,  NULL );

	xTaskCreate(
    setData
	,  "SetData"
	,  configMINIMAL_STACK_SIZE+200
	,  NULL
	,  1 
	,  NULL );
}

//Prepare Packets
void setSensorData(){
	//Create new struct copy of data and put the data from sensors into it
   struct sensors_data * pData = &data;
    pData->co2 = getCo2();
    pData->humidity = getHumidity();
    pData->temperature = getTemperature();
}

void runSetData(){
	xEventGroupWaitBits(
		EventGroupHandle,
		BIT_0 | BIT_1 | BIT_2,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	vTaskDelay(40);
	printf("--------------\n");
	printf("Environment start to set the data\n");
	setSensorData();
	xEventGroupSetBits(EventGroupHandle, BIT_4);
	vTaskDelay(50);
}

//1st Task to get Data from Sensors and prepare it into a Packet.
void setData(void * p){
	(void)p;
	for(;;){
		runSetData();
	}
 	
}

void runSendDataToQueue(){
	//Await for SetData task to be Done
	xEventGroupWaitBits(
			EventGroupHandle,
			BIT_4,
			pdTRUE,
			pdTRUE,
			portMAX_DELAY);
	//xQueueSend(xQueue2, (void*)&data,0);
	//Set timeout in queue so if its full it wont be blocked forever, but rather for only 1 milisecond
	if (xQueueSendToBack(xQueue2, (void*)&data, 1) != pdPASS) {
        //(queue is full), ignore and lose the packet.
    }
}

//2nd Task to send data to Queue
void sendDataToQueue(void * p){
	(void)p;
	xQueue2 = xQueueCreate(1,sizeof(data));
	for(;;){

   	runSendDataToqueue();
		
	}
}
