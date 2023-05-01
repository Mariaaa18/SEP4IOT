#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include "cotwo.h"
#include <mh_z19.h>
void runTaskIrl();

//If not work, change to pointer
uint16_t cotwo = 0;


void createCoTwo(){
    mh_z19_initialise(ser_USART3);
    xTaskCreate(
    runTaskIrl
	,  "CoTwoTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void runTaskIrl(){
    printf("RUN MTF");
    mh_z19_returnCode_t rc;
    for(;;){
        printf("Run Forrest Run (Inside Loop)");
        rc = mh_z19_takeMeassuring();
        vTaskDelay(100);
        if(rc != MHZ19_OK){
            printf("Co2 not measured.");
        }
        mh_z19_getCo2Ppm(&cotwo);
        printf("Hello, today gas chamber is at: %d", cotwo);
    }
}

void myCo2CallBack(uint16_t ppm){
    //If doesnt update, add here 
    //mh_z19_getCo2Ppm(&cotwo);
}

int getCoTwo(){
    return cotwo;
}
