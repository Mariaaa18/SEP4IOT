#include <stdio.h>
// #include <avr/io.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include "controllerSender.h"
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
#include "semphr.h"
#include "dataShared.h"

SemaphoreHandle_t mutex;
// struct that will keep the data to be sent to the queue
struct sensors_data dataS;


/*-----------------------------------------------------------*/

void createMutex(){
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        // Mutex creation failed
        // Handle the error
    }
}

// Prepare Packets
struct sensors_data* setSensorData()
{ 
    struct sensors_data* pData = &dataS;
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        // Create new struct copy of data and put the data from sensors into it
        printf("\n ----------Enter SetData \n");
        pData->co2 = getCoTwo();
        pData->humidity = getHumidity();
        vTaskDelay(10);
        printf("----------Humidity : %d \n",pData->humidity);
        vTaskDelay(10);
        printf("----------Humidity from Get : %d \n",getHumidity());
        vTaskDelay(10);
        printf("------------Temperature : %d \n ", pData->temperature);
        vTaskDelay(10);
        printf("------------Temperature from Get : %d \n ", getTemperature());
        pData->temperature = getTemperature();
        xSemaphoreGive(mutex);  
    }
    return pData;
}


 struct sensors_data* getSensorData(){
    struct sensors_data* pData= &dataS;
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        xSemaphoreGive(mutex);
    }  
    return pData;
} 


