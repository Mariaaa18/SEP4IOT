typedef struct dataShared* dataShared_t;
/*void createMutex();
void setSensorData();*/
dataShared_t dataShared_create(uint16_t co2, uint16_t humidity, int temperature);
void dataShared_setValues(uint16_t co2, uint16_t humidity, int temperature, dataShared_t self);
uint16_t dataShared_getCo2(dataShared_t self);
int16_t dataShared_getTemperature(dataShared_t self);
uint16_t dataShared_getHumidity(dataShared_t self);