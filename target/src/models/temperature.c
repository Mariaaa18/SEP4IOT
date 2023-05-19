#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "../controllers/dataShared.h"

#include <queue.h>
#include <lora_driver.h>
#include <status_leds.h>
#include "temperature.h"
#include <hih8120.h>
#include <rc_servo.h>

#define BIT_0 (1 << 0)

extern EventGroupHandle_t _myEventGroupSender;

void runTaskTemperature();

// If not work, change to pointer
uint16_t temperature = 0;




void createTemperature()
{
    if (HIH8120_OK != hih8120_initialise())
    {
        // Driver initialised OK
        printf("temp  sensor not initialized");
        // Always check what hih8120_initialise() returns
    }
    //init servo
	rc_servo_initialise();

    xTaskCreate(
        runTaskTemperature, "TemperatureTask" // A name just for humans
        ,
        configMINIMAL_STACK_SIZE // This stack size
        ,
        NULL, 3 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL);
    // Inject Callback Reference(Insisde the runTaskIrl, whenever there is a new measuring, we call the callback and send in args the new reading, which then the callback will replace cotwo with that reading)
}

void runTaskTemperature()
{
   
    // printf("Inside the temperature measuring task \n");

    for (;;)
    {
        //  printf("Inside temp mesaure loop \n");
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
        printf("Temp: %d\n", temperature);
        xEventGroupSetBits(_myEventGroupSender, BIT_0);
        vTaskDelay(1);
        printf("heap space remaining: %d \n",xPortGetFreeHeapSize());

        // \param[in] servoNo The servo to set the position for (0:J14 (Servo#0) 1:J13 (Servo#1))
        //\param[in] percent The position to set the servo to [-100 .. 100]
       // void rc_servo_setPosition(uint8_t servoNo, int8_t percent);
        //servo
       

        
      //  printf("after setting servo ");
      vTaskDelay(2500);


        //  printf("Temperature bit %d is set and try to send.\n",BIT_0);
        // delay 25sec
        
    }
}
int getTemperature()
{
    return temperature;
}