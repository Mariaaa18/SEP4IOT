#include <stddef.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include <lora_driver.h>
#include <status_leds.h>
#include "humidity.h"
#include "cotwo.h"
#include "temperature.h"
#include <hih8120.h>

#define BIT_2 (1 << 2)
extern EventGroupHandle_t _myEventGroupSender;

// Include other 2 tasks in order for queue to work, as well as main/environment.

void runTaskHumidity();

// If not work, change to pointer
uint16_t humidity = 0;

void createHumidity()
{

    xTaskCreate(
        runTaskHumidity, "HumidityTask" // A name just for humans
        ,
        configMINIMAL_STACK_SIZE + 200 // This stack size
        ,
        NULL, 1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
    // Inject Callback Reference(Insisde the runTaskIrl, whenever there is a new measuring, we call the callback and send in args the new reading, which then the callback will replace cotwo with that reading)
}

// Set arg with callback reference;
void runTaskHumidity()
{

    for (;;)
    {
        if (HIH8120_OK != hih8120_wakeup())
        {
            // Something went wrong
            // Investigate the return code further
        }

        vTaskDelay(100); // After the hih8120_wakeup() call the sensor will need minimum 50 ms to be ready!
        if (HIH8120_OK != hih8120_measure())
        {
            // Something went wrong
            // Investigate the return code further
        }

        vTaskDelay(100); // After the hih8120_measure() call the two wire inteface (TWI) will need minimum 1 ms to fetch the results from the sensor!

        humidity = hih8120_getHumidity();
        printf("Hello, today humidity at: %d", humidity);
        xEventGroupSetBits(_myEventGroupSender, BIT_2);
        // delay 25sec
        vTaskDelay(25000);
        // Get the queue now.
        // Create a messaage (value of co2)
        // point to message
        // xMessage *pxPointerMessage = &myMessage
        // Send it to the queue (xQueueSend(The handle of the queue , The ADDRESS of the variable that holds the message, Delay))
    }
}

// Inside the Callback, we will set the global variable to be equal the argument passed(or in other words, the latest measuring).
// We use this if we want to get a new measuring each time that there is a new measuring read, otherwise wed set a timeout and wait for a new value.
void myHumidityCallBack(uint16_t ppm)
{
    // If doesnt update, add here
    // mh_z19_getCo2Ppm(&cotwo);
}

int getHumidity()
{
    return humidity;
}

// In the other sensor classes:
/*
    have a pointer to the xMessage*
    As for xQueueReceive(Queue handler, pointer to message, delay)
    Two options: Either add all the sensors values like a train passing by stops, or only send message to Main and somehow handle it inside of Main.

*/

// Event Groups
/*
    Declare global
    #define BIT_TASK_A_READY(1 << 0)
    #define BIT_TASK_B_READY (1 << 1)

    Define the event group some where in main or in an initialization task
    _myEventGroup = xEventGroupCreate()

    To set bits in group
    xEventGroupSetBits(_myEventGroup, BIT_TASK_A_READY);

    Wait for event bits to be set in Group
    xEventGroupWaitBits(
        _myEventGroup, //Even group being tested
        BIT_TASK_B_READY, // Bits to wait for
        pdTRUE, //Clear bits before return
        pdTRUE, //Wait for bits to be set
        portMAX_DELAY //Maximum time to wait
    )
*/