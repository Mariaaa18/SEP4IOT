#include <stdio.h>
#include <stdlib.h>
// #include <avr/io.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include "controllerSender.h"
#include "../models/cotwo.h"
#include "../models/humidity.h"
#include "../models/temperature.h"
#include "semphr.h"
#include "dataShared.h"

typedef struct dataShared {
    uint16_t co2;
    uint16_t humidity;
    int16_t temperature;
} dataShared;

static dataShared_t _newDataShared;


dataShared_t dataShared_create(uint16_t co2, uint16_t humidity, int16_t temperature){
    //printf("I. Co2: %d \n", co2);
    _newDataShared = calloc(sizeof(dataShared),1);
    
    if(NULL==_newDataShared){
        printf("return null");
        return NULL;
    }


    _newDataShared->co2=co2;
    //printf("I. Co2: %d \n", _newDataShared->co2);
    _newDataShared->humidity=humidity;
    _newDataShared->temperature=temperature;
    return _newDataShared; 
}

void dataShared_destroy(dataShared_t self)
{
    if(NULL!=self){
        free(self);
    }
}

void dataShared_setValues(uint16_t co2, uint16_t humidity, int16_t temperature, dataShared_t self){
 
    self->humidity=humidity;
    self->co2= co2;
    self->temperature= temperature;
   
}



int16_t dataShared_getTemperature(dataShared_t self){
    return self->temperature;
}

uint16_t dataShared_getCo2(dataShared_t self){
    printf("M. Co2: %d \n", self->co2);
    return self->co2;
}

uint16_t dataShared_getHumidity(dataShared_t self){

    return self->humidity;
}



   






