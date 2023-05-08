#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include <lora_driver.h>
#include <status_leds.h>
#include "temperature.h"
#include <hih8120.h>

#define BIT_0 (1 << 0)

extern EventGroupHandle_t _myEventGroupSender;

void runTaskTemperature();

// If not work, change to pointer
uint16_t temperature = 0;

void createTemperature()
{
    if (HIH8120_OK == hih8120_initialise())
    {
        // Driver initialised OK
        printf("temp sensor initialized");
        // Always check what hih8120_initialise() returns
    }
    xTaskCreate(
        runTaskTemperature, "TemperatureTask" // A name just for humans
        ,
        configMINIMAL_STACK_SIZE + 200 // This stack size
        ,
        NULL, 1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
    // Inject Callback Reference(Insisde the runTaskIrl, whenever there is a new measuring, we call the callback and send in args the new reading, which then the callback will replace cotwo with that reading)
}

void runTaskTemperature()
{
    printf("Inside the temperature measuring task \n");

    for (;;)
    {
        printf("Inside temp mesaure loop \n");
        if (HIH8120_OK != hih8120_wakeup())
        {
            // Something went wrong
            // Investigate the return code further
            printf("Error in temp wake up: %d", hih8120_wakeup());
        }

        // If the callback is well implemented, we shouldnt require the TaskDelay
        vTaskDelay(100);
        if (HIH8120_OK != hih8120_measure())
        {
            printf("Error in temp measure method");
            // Something went wrong
            // Investigate the return code further
        }

        // cahnge to uint if doesnt work
        temperature = hih8120_getTemperature_x10();
        printf("Hello, inside temp measure loop with temp: %d", temperature);
        xEventGroupSetBits(_myEventGroupSender, BIT_0);
        // delay 25sec
        vTaskDelay(25000);
    }
}
int getTemperature()
{
    return temperature;
}