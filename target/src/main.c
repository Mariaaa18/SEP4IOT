
/*
* main.c

* Author : Group-2

*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <stdio_driver.h>
#include <serial.h>

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

#include <hih8120.h>
#include "event_groups.h"
#include "controllers/controllerSender.h"
#include "models/cotwo.h"
#include "models/humidity.h"
#include "models/temperature.h"
#include "controllers/dataShared.h"

// define queue

EventGroupHandle_t _myEventGroupSender = NULL;
QueueHandle_t xQueue_DownLink;
struct sensors_data dataM;
// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	createMutex();
	// Make this into queue class
	_myEventGroupSender = xEventGroupCreate();
	if (_myEventGroupSender == NULL)
	{
		printf("Failed to create mutex\n");
	}

	// Set xMessage. In our example this Message could be a int to say the task if it can run or not.
	// Create tasks
	createCoTwo();
	createHumidity();
	createTemperature();
	controllerSendTask();
  xQueue_DownLink = xQueueCreate(1, sizeof(dataM));
	// xQueueCreate( Number of items a queue can hold , Size of each item , vTaskStartScheduler() )
	//_myEventGroupSender = xEventGroupCreate();
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// Let's create some tasks
	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
	// humidity inizialiser
	if (HIH8120_OK == hih8120_initialise())
	{

		// Driver initialised OK
		// Always check what hih8120_initialise() returns
	}
}

/*-----------------------------------------------------------*/
int main(void)
{
	printf("In main before Initialize!!\n");
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");

	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.
	/* Replace with your application code */
	while (1)
	{
	}
}
