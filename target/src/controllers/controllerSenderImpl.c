
#include <stdio.h>
// #include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <event_groups.h>
#include "controllerSender.h"
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
#include "dataShared.h"
#include "semphr.h"
// define queue
QueueHandle_t xQueue2;

// struct that will keep the data to be sent to the queue
struct sensors_data* dataC;
extern EventGroupHandle_t _myEventGroupSender;


// sensor bits
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)

/*-----------------------------------------------------------*/



void runSetData()
{
	
	//printf("before the event group------\n");
	xEventGroupWaitBits(
		_myEventGroupSender,
		BIT_0 | BIT_1 | BIT_2,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	vTaskDelay(40);
	
	dataC = setSensorData(); //this is form the mutex;
	printf("-->event group triggered \n");
	
	

	vTaskDelay(75);
	if (xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS)
	{
		//printf("queue is full");
	}
}

// 1st Task to get Data from Sensors and prepare it into a Packet.
void setData(void *p)
{
	(void)p;
	for (;;)
	{
		
		runSetData();
	}
}

void controllerSendTask()
{
	xQueue2 = xQueueCreate(1, sizeof(dataC));
	xTaskCreate(
		setData, "SetData", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	

	 
}

