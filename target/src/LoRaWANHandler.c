
/*
 * loraWANHandler.c
 *
 * Created: 12/04/2019 10:09:05
 *  Author: IHA
 */
#include <stddef.h>
#include <stdio.h>


#include <ATMEGA_FreeRTOS.h>
#include <queue.h>
#include <lora_driver.h>
#include <status_leds.h>
#include <stream_buffer.h>
#include <message_buffer.h>
#include "controllers/dataShared.h"
#include "models/actuator.h"
// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "1AB7F2972CC78C9A"
#define LORA_appKEY "6C7EF7F5BC5266D1FAEE88AF7EA9BABD"

void lora_handler_task(void *pvParameters);

static lora_driver_payload_t _uplink_payload;
static lora_driver_payload_t downlinkPayload;
extern QueueHandle_t xQueue2;



extern MessageBufferHandle_t downLinkMessageBufferHandle;
extern QueueHandle_t xQueue_DownLink;

struct sensors_data* data;
//struct sensors_data downData;
//struct sensors_data* downDataP;



void lora_handler_initialise(UBaseType_t lora_handler_task_priority)
{
	xTaskCreate(
		lora_handler_task, "LRHand" // A name just for humans
		,
		configMINIMAL_STACK_SIZE + 200 // This stack size can be checked & adjusted by reading the Stack Highwater
		,
		NULL, lora_handler_task_priority // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		,
		NULL);

		
}

static void _lora_setup(void)
{
	
	char _out_buf[20];
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));

	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n", lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI, LORA_appKEY, _out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;


	do
	{
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if (rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
			
			
			
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task(void *pvParameters)
{
	initServo();
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);
	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;

	
	downlinkPayload.portNo = 1;
	downlinkPayload.len = 6;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();

	

		if(downLinkMessageBufferHandle == NULL){
			printf("messgage buffer is null");
		}

	printf("I am in LoraWAN ---before-- for Loop----\n");
	// testing servo

	vTaskDelay(100);
	

	for (;;)
	{
		
		setServoOptimal();
	
		

		printf("I am in LoraWAN before waiting time----\n");
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		printf("------I am in LoraWAN before queue----\n");
		xQueueReceive(xQueue2, &data, portMAX_DELAY);
		printf("------I am in LoraWAN after queue----\n");


		if(downLinkMessageBufferHandle == NULL){
			printf("messgage buffer is null insede the loop");
		}

		lora_driver_returnCode_t rc;
			//init var for downlink
		uint16_t maxHumSetting =0; // Max Humidity
        int16_t maxTempSetting= 0; // Max Temperature
		uint16_t maxCo2Setting = 0;

		printf("L. Humidity: %d \n", data->humidity);
		

		// Some dummy payload
		uint16_t hum = data->humidity;	 // Dummy humidity
		int16_t temp = data->temperature; // Dummy temp
		uint16_t co2_ppm = data->co2;	 // Dummy CO2

		_uplink_payload.bytes[0] = hum >> 8;
		_uplink_payload.bytes[1] = hum & 0xFF;
		_uplink_payload.bytes[2] = temp >> 8;
		_uplink_payload.bytes[3] = temp & 0xFF;
		_uplink_payload.bytes[4] = co2_ppm >> 8;
		_uplink_payload.bytes[5] = co2_ppm & 0xFF;

		

		status_leds_shortPuls(led_ST4); // OPTIONAL

		if ((rc = lora_driver_sendUploadMessage(false, &_uplink_payload)) == LORA_MAC_TX_OK )
		{
			vTaskDelay(500);
			
			// The uplink message is sent and there is no downlink message received
			printf("----message uploaded no download link--- \n");
	
		 	//	printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
		 	//xQueueSend(xQueue_DownLink, (void *)&data, portMAX_DELAY);
		}
		else if (rc == LORA_MAC_RX)
		{
			
		

			printf("message has downLink \n");
			// The uplink message is sent and a downlink message is received
			
			vTaskDelay(10);

			xMessageBufferReceive(downLinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);

			vTaskDelay(500);
			
			// Just for Debug	
       		 printf("DOWN LINK: from port: %d with %d bytes received! \n", downlinkPayload.portNo, downlinkPayload.len); 
            if (6 <= downlinkPayload.len) // Check that we have got the expected 4 bytes
            {
       			// decode the payload into our variales
                maxHumSetting = (downlinkPayload.bytes[0] << 8) + downlinkPayload.bytes[1];
                maxTempSetting = (downlinkPayload.bytes[2] << 8) + downlinkPayload.bytes[3];
				maxCo2Setting = (downlinkPayload.bytes[4] << 8) + downlinkPayload.bytes[5];
				vTaskDelay(100);

			
				
			data->co2 = maxCo2Setting;
			data->humidity = maxHumSetting;
			data->temperature = maxTempSetting;

		

			 //this if when we have the reciever controller

			if(xQueueSend(xQueue_DownLink, (void *)&data, portMAX_DELAY) !=pdPASS){
				printf("Failed to send item");
			}

		

            }
			printf("recieved message hum: %d temp(from struct): %d  co2: %d \n",maxHumSetting,data->temperature,maxCo2Setting);

			//this probaly needs some refactoring

	}
		}
		

	
		
}


