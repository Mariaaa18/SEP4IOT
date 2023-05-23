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

typedef struct sensors_data {
    uint16_t co2;
    uint16_t humidity;
    int temperature;
} dataShared;


dataShared_t dataShared_create(uint16_t co2, uint16_t humidity, int temperature){
    dataShared_t _newDataShared= calloc(sizeof(dataShared),1);
    if(NULL==_newDataShared){
        return NULL;
    }

    _newDataShared->co2=co2;
    _newDataShared->humidity=humidity;
    _newDataShared->temperature= temperature;
    return _newDataShared; 
    }

void dataShared_destroy(dataShared_t self)
{
    if(NULL!=self){
        free(self);
    }
}

void dataShared_setValues(uint16_t co2, uint16_t humidity, int temperature, dataShared_t self){
 
    self->co2= co2;
    self-> humididty=humidity;
    self-> temperature= temperature;
   
}

uint16_t dataShared_getCo2(dataShared_t self){

    return self->co2;
}

uint16_t dataShared_getTemperature(dataShared_t self){

    return self->temperature;
}
int dataShared_getHumidity(dataShared_t self){

    return self->humidity;
}



   






