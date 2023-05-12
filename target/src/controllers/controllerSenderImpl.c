
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

/* Prepare Packets
void setSensorData()
{
	// Create new struct copy of data and put the data from sensors into it
	struct sensors_data *pData = &dataC;
	pData->co2 = getCoTwo();
	pData->humidity = getHumidity();
	pData->temperature = getTemperature();
}*/

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
	printf("bit 0 is :%d || bit 1 is:%d || bit 2 is:%d ||, \n", BIT_0, BIT_1, BIT_2);
	dataC = setSensorData();

	vTaskDelay(50);
	if (xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS)
	{
		printf("queue is full");
		//(queue is full), ignore and lose the packet.
	}
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

void controllerSendTask()
{
	xQueue2 = xQueueCreate(1, sizeof(dataC));
	xTaskCreate(
		setData, "SetData", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);
}

