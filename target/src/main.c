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
#include <stream_buffer.h>
#include <message_buffer.h>
#include "event_groups.h"
#include "controllers/controllerSender.h"
#include "controllers/controllerReceiver.h"
#include "models/cotwo.h"
#include "models/humidity.h"
#include "models/temperature.h"
#include "controllers/dataShared.h"
#include "models/mystruct.h"

// define queue
EventGroupHandle_t _myEventGroupSender = NULL;
MessageBufferHandle_t downLinkMessageBufferHandle = NULL;
QueueHandle_t xQueue_DownLink = NULL;

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

	createCoTwo();
	createHumidity();
	createTemperature();
	controllerSendTask();
	controllerReceiveTask();
	

	xQueue_DownLink = xQueueCreate(1, sizeof(struct MyData));


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
	//lora_driver_initialise(1, NULL);

	// Here I make room for two downlink messages in the message buffer
	
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2); // Here I make room for two downlink messages in the message buffer
    lora_driver_initialise(1, downLinkMessageBufferHandle);  // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
	
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
	initialiseSystem();
	printf("Program Started!!\n");
    vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.
	
    
	
	while (1)
	{
		
	}
}

