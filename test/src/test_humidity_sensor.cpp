#include "gtest/gtest.h"
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <humidity.h>
#include <hih8120.h>
#include <serial.h>
#include <rc_servo.h>
}

//float hih8120_getHumidity(void);
//uint16_t hih8120_getHumidityPercent_x10(void);

FAKE_VALUE_FUNC(float, hih8120_getHumidity);


// FAKE_VOID_FUNC(run_coTwo);


class HumidityTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        RESET_FAKE(xEventGroupCreate);
        RESET_FAKE(xEventGroupSetBits);
        RESET_FAKE(hih8120_getHumidity);
        FFF_RESET_HISTORY();
        
    }
};

TEST_F(HumidityTest, create_hum_task)
{

    createHumidity();

    ASSERT_EQ(reinterpret_cast<void (*)(void *)>(xTaskCreate_fake.arg0_val), reinterpret_cast<void (*)(void *)>(&runTaskHumidity));
}

TEST_F(HumidityTest, RunHum_success_full_measurement)
{
    hih8120_getHumidity_fake.return_val = 200;
    /*mh_z19_getCo2Ppm_fake.custom_fake = [](uint16_t *ppm)
    {
        *ppm = 1000; // Set the CO2 value to 1000 ppm
        return MHZ19_OK;
    };*/

    createHumidity();

    runHumidity();

    ASSERT_EQ(hih8120_getHumidity_fake.call_count, 1);
    ASSERT_EQ(xEventGroupSetBits_fake.call_count, 1);

    ASSERT_EQ(getHumidity(), 200);
}
