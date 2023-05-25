#include <stdio.h>
// #include <avr/io.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include "../../drivers/include/controllerSender.h"
#include "../../drivers/include/cotwo.h"
#include "../../drivers/include/humidity.h"
#include "../../drivers/include/temperature.h"
#include "semphr.h"
#include "../../drivers/include/dataShared.h"

SemaphoreHandle_t mutex;
// struct that will keep the data to be sent to the queue
struct sensors_data dataS;



/*-----------------------------------------------------------*/

void createMutex(){

    //printf("initializing mutex \n");
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        // Mutex creation failed
        printf("mutex is null");

        // Handle the error
    }
}



// Prepare Packets
struct sensors_data* setSensorData()
{ 

    struct sensors_data* pData = &dataS;
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
	// Create new struct copy of data and put the data from sensors into it
	
	pData->co2 = getCoTwo();
	pData->humidity = getHumidity();
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


