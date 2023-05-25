#include "gtest/gtest.h"
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <controllerReceiver.h>
#include <lora_driver.h>
#include <serial.h>
#include <actuator.h>
#include <dataShared.h>
}

FAKE_VALUE_FUNC0(struct sensors_data *, getSensorData);
FAKE_VALUE_FUNC0(struct sensors_data *, setSensorData);
FAKE_VOID_FUNC(setServoHigh);
FAKE_VOID_FUNC(setServoLow);
FAKE_VOID_FUNC(initServo);

QueueHandle_t xQueue_DownLink = NULL;

class ControllerReceiveTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        RESET_FAKE(setSensorData);
        RESET_FAKE(setServoHigh);
        RESET_FAKE(setServoLow);
        RESET_FAKE(initServo);
        RESET_FAKE(getSensorData);
        RESET_FAKE(xQueueCreate);
        RESET_FAKE(xQueueSend);
        RESET_FAKE(xQueueReceive);
        RESET_FAKE(xTaskCreate);
        FFF_RESET_HISTORY();
    }
};

TEST_F(ControllerReceiveTest, create_task)
{

    controllerReceiveTask();
    ASSERT_EQ(xTaskCreate_fake.call_count, 1);
    ASSERT_EQ(reinterpret_cast<void (*)(void *)>(xTaskCreate_fake.arg0_val), reinterpret_cast<void (*)(void *)>(&setRetriever));
    ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "setRetriever", 1), 0);
}

TEST_F(ControllerReceiveTest, get_sensor_data)
{
    // Create a fake sensor data object
    struct sensors_data fakeData;
    fakeData.co2 = 50;
    fakeData.humidity = 70;
    fakeData.temperature = 25;

    // Set up the behavior of the getSensorData fake function
    getSensorData_fake.return_val = &fakeData;

    // Call the function under test
    struct sensors_data *result = getSensorData();

    // Verify the expected behavior
    ASSERT_EQ(result->co2, fakeData.co2);
    ASSERT_EQ(result->humidity, fakeData.humidity);
    ASSERT_EQ(result->temperature, fakeData.temperature);
    ASSERT_NE(result->co2, fakeData.humidity);
}
