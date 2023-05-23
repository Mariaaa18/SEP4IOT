
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "humidity.h"
#include <hih8120.h>

#define BIT_2 (1 << 2)

// If not work, change to pointer
uint16_t humidity = 0;

void createHumidity()
{

    xTaskCreate(
        runTaskHumidity, "HumidityTask" // A name just for humans
        ,
        configMINIMAL_STACK_SIZE // This stack size
        ,
        NULL, 3 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
    // Inject Callback Reference(Insisde the runTaskIrl, whenever there is a new measuring, we call the callback and send in args the new reading, which then the callback will replace cotwo with that reading)
}

// Set arg with callback reference;

void runTaskHumidity()
{
    rcH = hih8120_wakeup();

    if (HIH8120_OK != rcH)
    {
        printf("Error in humidity wake up: %d", rcH);
        if (HIH8120_DRIVER_NOT_INITIALISED == rcH)
        {
            hih8120_initialise();
        }
    }

    vTaskDelay(57);
    if (HIH8120_OK != hih8120_measure())
    {
        printf("Error to mesure humidity");
    }

    vTaskDelay(55);

    humidity = hih8120_getHumidity();
    printf("Hum: %d", humidity);
    xEventGroupSetBits(_myEventGroupSender, BIT_2);
    // delay 25sec
    vTaskDelay(2500);
}
void humidity_task(void *p)
{
    (void)p;

    for (;;)
    {
        runTaskHumidity();
    }
}

void createHumidityTask()
{

    xTaskCreate(
        humidity_task, "HumidityTask",
        configMINIMAL_STACK_SIZE,
        NULL,
         1,
        NULL);
}

uint16_t getHumidity()
{
    return humidity;
}

