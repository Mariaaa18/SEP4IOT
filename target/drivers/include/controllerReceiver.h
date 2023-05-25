#pragma once
#include <stdint.h>

void controllerReceiveTask();
void retrieveQueueData();
void setCurrentValue();
void actOnTemperature();
void actOnCo2();
void runRetriever();
void setRetriever(void *p);
