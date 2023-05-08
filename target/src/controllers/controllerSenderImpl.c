#include <stdio.h>
//#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerSender.h"
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"

// define queue
QueueHandle_t xQueue2;

// struct that will keep the data to be sent to the queue
struct sensors_data dataC;
extern EventGroupHandle_t _myEventGroupSender;
// sensor bits
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
// setData bit
#define BIT_4 (1 << 4)

/*-----------------------------------------------------------*/


// Prepare Packets
void setSensorData()
{
	// Create new struct copy of data and put the data from sensors into it
	struct sensors_data *pData = &dataC;
	pData->co2 = getCoTwo();
	pData->humidity = getHumidity();
	pData->temperature = getTemperature();
}

void runSetData()
{
	printf("before the event group------\n");
	xEventGroupWaitBits(
		_myEventGroupSender,
		BIT_0 | BIT_1 | BIT_2,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	vTaskDelay(40);
	printf("--------------\n");
	printf("Environment start to set the data\n");
	printf("bit 0 is :%d || bit 1 is:%d || bit 2 is:%d ||, \n",BIT_0,BIT_1,BIT_2 );
	setSensorData();
	xEventGroupSetBits(_myEventGroupSender, BIT_4);
	vTaskDelay(50);
}

// 1st Task to get Data from Sensors and prepare it into a Packet.
void setData(void *p)
{
	(void)p;
	for (;;)
	{
		printf("SetData---\n");
		runSetData();
	}
}

void runSendDataToQueue()
{
	printf("inside send data to queuu-->");
	// Await for SetData task to be Done
	xEventGroupWaitBits(
		_myEventGroupSender,
		BIT_4,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	// xQueueSend(xQueue2, (void*)&data,0);
	// Set timeout in queue so if its full it wont be blocked forever, but rather for only 1 milisecond
	if (xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS)
	{
		printf("queue is full");
		//(queue is full), ignore and lose the packet.
	}
	printf("data added to queue-->");
	vTaskDelay(50);
}

// 2nd Task to send data to Queue
void sendDataToQueue(void *p)
{
	(void)p;
	
	for (;;)
	{
	
		runSendDataToQueue();

	}
}

void testTask(){
	for(;;){
		vTaskDelay(1000);
		printf("inside test task controller \n");
	}

}

void controllerSenderTask()
{
	// Create tasks
	xQueue2 = xQueueCreate(1, sizeof(dataC));
	xTaskCreate(
		sendDataToQueue, "sendData", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);
	/* 	xTaskCreate(
		testTask, "testTask", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL); */
	
 /**
	


	

	
		*/
	

}

void controllerSetTask(){
	xTaskCreate(
		setData, "SetData", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);
}
