#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include "cotwo.h"
#include <mh_z19.h>

#define BIT_TASK_H_READY(1 << 2)
static mh_z19_returnCode_t rc;
static uint16_t cotwo = 0;
extern EventGroupHandle_t _myEventGroupSender;

void coTwo_task(void *p)
{
    (void)p;

    mh_z19_initialise(ser_USART3);

    mh_z19_returnCode_t rc;
    for (;;)
    {
        run_coTow();
    }
}
void run_coTow()
{
    printf("Run Forrest Run (Inside Loop)");
    rc = mh_z19_takeMeassuring();

    vTaskDelay(100);
    if (rc != MHZ19_OK)
    {
        // if there is a failure we could send something to the loraWAN? check better
        printf("Co2 not measured.");
    }

    mh_z19_getCo2Ppm(&cotwo);
    printf("Hello, today gas chamber is at: %d", cotwo);

    // delay 25sec
    vTaskDelay(25000);
}
void createCoTwo()
{
    // maybe create the task in main
    xTaskCreate(
        coTwo_task, "CoTwoTask", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);
}

int getCoTwo()
{
    return cotwo;
}
