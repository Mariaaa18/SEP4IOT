#pragma once
#include <stdint.h>

void hal_create(uint8_t portNo);
int16_t hal_getTemperature(uint8_t channel);
