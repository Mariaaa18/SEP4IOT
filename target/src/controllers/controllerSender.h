#pragma once
#include <stdint.h>

struct sensors_data
{
	uint16_t co2;
	uint16_t humidity;
	uint16_t temperature;
};

void controllerSendTask();
