#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include <lora_driver.h>
#include <status_leds.h>
#include "temperature.h"
#include <hih8120.h>

#define BIT_0 (1 << 0)

extern EventGroupHandle_t _myEventGroupSender;
static hih8120_driverReturnCode_t rcT;
// If not work, change to pointer
int16_t temperature = 0;

void runTemperatureTask()
{
    rcT = hih8120_wakeup();
    if (HIH8120_OK != rcT)
    {

        printf("Error in temp wake up: %d", rcT);
        if (HIH8120_DRIVER_NOT_INITIALISED == rcT)
        {
            hih8120_initialise();
        }
    }
    vTaskDelay(55);

    rcT = hih8120_measure();
    if (HIH8120_OK != rcT)
    {
        printf("Error in temp measure method: %d\n", rcT);
        if (HIH8120_DRIVER_NOT_INITIALISED == rcT)
        {
            hih8120_initialise();
        }
    }
    vTaskDelay(55);
    temperature = hih8120_getTemperature_x10();
    printf("Temp: %d\n", temperature);
    xEventGroupSetBits(_myEventGroupSender, BIT_0);
    // delay 25sec
    vTaskDelay(2500);
}
void temperature_task(void *p)
{
    (void)p;
    for (;;)
    {
        runTemperatureTask();
    }
}
void createTemperature()
{

    xTaskCreate(
        temperature_task, "TemperatureTask",
        configMINIMAL_STACK_SIZE + 200,
        NULL, 1,
        NULL);
}
int16_t getTemperature()
{
    return temperature;
}