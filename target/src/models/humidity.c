#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "humidity.h"
#include <hih8120.h>

#define BIT_2 (1 << 2)

extern EventGroupHandle_t _myEventGroupSender;
static hih8120_driverReturnCode_t rcH;
static uint16_t humidity = 0;

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

void createHumidity()
{

    xTaskCreate(
        humidity_task, "HumidityTask",
        configMINIMAL_STACK_SIZE + 200,
        NULL, 1,
        NULL);
}

uint16_t getHumidity()
{
    return humidity;
}