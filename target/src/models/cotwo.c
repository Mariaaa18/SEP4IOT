#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include "cotwo.h"
#include <mh_z19.h>
//Include other 2 tasks in order for queue to work, as well as main/environment.

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
    //Inject Callback Reference(Insisde the runTaskIrl, whenever there is a new measuring, we call the callback and send in args the new reading, which then the callback will replace cotwo with that reading)
}

//Set arg with callback reference;
void runTaskIrl(){
    printf("RUN MTF");
    mh_z19_returnCode_t rc;
    for(;;){
        printf("Run Forrest Run (Inside Loop)");
        rc = mh_z19_takeMeassuring();
        
        //If the callback is well implemented, we shouldnt require the TaskDelay
        vTaskDelay(100);
        if(rc != MHZ19_OK){
            printf("Co2 not measured.");
        }
        //Here we call the callback reference passed in the arguments, and we send the coTwo reading with it.

        mh_z19_getCo2Ppm(&cotwo);
        printf("Hello, today gas chamber is at: %d", cotwo);
        //Get the queue now.
        //Create a messaage (value of co2)
        //point to message
        //xMessage *pxPointerMessage = &myMessage
        //Send it to the queue (xQueueSend(The handle of the queue , The ADDRESS of the variable that holds the message, Delay))
    }
}


//Inside the Callback, we will set the global variable to be equal the argument passed(or in other words, the latest measuring).
//We use this if we want to get a new measuring each time that there is a new measuring read, otherwise wed set a timeout and wait for a new value.
void myCo2CallBack(uint16_t ppm){
    //If doesnt update, add here 
    //mh_z19_getCo2Ppm(&cotwo);
}

int getCoTwo(){
    return cotwo;
}

//In the other sensor classes:
/*
    have a pointer to the xMessage*
    As for xQueueReceive(Queue handler, pointer to message, delay)
    Two options: Either add all the sensors values like a train passing by stops, or only send message to Main and somehow handle it inside of Main.
    
*/


//Event Groups
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