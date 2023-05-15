#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include <lora_driver.h>
#include <status_leds.h>
#include "cotwo.h"
#include <mh_z19.h>

#define BIT_1 (1 << 1)
static mh_z19_returnCode_t rc;
static uint16_t cotwo = 0;
extern EventGroupHandle_t _myEventGroupSender;

void runCoTowTask()
{

    //  printf("Run Forrest Run (Inside Loop)\n");
    rc = mh_z19_takeMeassuring();

    vTaskDelay(110);
    if (rc != MHZ19_OK)
    {
        // if there is a failure we could send something to the loraWAN? check better
        printf("Co2 not measured. \n");
        mh_z19_initialise(ser_USART3);
    }

    mh_z19_getCo2Ppm(&cotwo);
    printf("CO2: %d \n", cotwo);
    xEventGroupSetBits(_myEventGroupSender, BIT_1);
    // delay 25sec
    vTaskDelay(2500);
}
void coTwo_task(void *p)
{
    (void)p;

    for (;;)
    {
        runCoTowTask();
    }
}

void createCoTwoTask()
{

    xTaskCreate(
        coTwo_task, "CoTwoTask", configMINIMAL_STACK_SIZE + 200, NULL, 1, NULL);
}

// Todo check with teacher how we can set the Bit from here if at the end we use callback
/* void myCo2CallBack(uint16_t ppm)
{

} */
void cotwo_sensorInit()
{
    mh_z19_initialise(ser_USART3);

    // mh_z19_injectCallBack(myCo2CallBack);
}

uint16_t getCoTwo()
{
    return cotwo;
}
