#include "gtest/gtest.h"
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <temperature.h>
#include <hih8120.h>
#include <serial.h>
#include <rc_servo.h>
}

/*hih8120_driverReturnCode_t hih8120_wakeup(void);
bool hih8120_isReady(void);
hih8120_driverReturnCode_t hih8120_initialise(void); 
hih8120_driverReturnCode_t hih8120_measure(void);
int16_t hih8120_getTemperature_x10(void);*/

FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);
FAKE_VALUE_FUNC(bool, hih8120_isReady);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_initialise);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);
FAKE_VALUE_FUNC(int16_t, hih8120_getTemperature_x10);
FAKE_VOID_FUNC(rc_servo_initialise)


// FAKE_VOID_FUNC(run_coTwo);


class TemperatureTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

        RESET_FAKE(hih8120_initialise);
        RESET_FAKE(hih8120_wakeup);
        RESET_FAKE(xEventGroupCreate);
        RESET_FAKE(xEventGroupSetBits);
        RESET_FAKE(hih8120_isReady);
        RESET_FAKE(hih8120_measure);
        RESET_FAKE(hih8120_getTemperature_x10);
        RESET_FAKE(rc_servo_initialise);
        FFF_RESET_HISTORY();
        
    }
};

TEST_F(TemperatureTest, create_temp_task)
{
    hih8120_measure_fake.return_val = HIH8120_OK;

    createTemperature();

    ASSERT_EQ(reinterpret_cast<void (*)(void *)>(xTaskCreate_fake.arg0_val), reinterpret_cast<void (*)(void *)>(&runTaskTemperature));
}

TEST_F(TemperatureTest, RunTemp_success_full_measurement)
{
    hih8120_initialise_fake;
    hih8120_wakeup_fake;
    hih8120_measure_fake.return_val = HIH8120_OK;
    hih8120_getTemperature_x10_fake.return_val = 200;
    /*mh_z19_getCo2Ppm_fake.custom_fake = [](uint16_t *ppm)
    {
        *ppm = 1000; // Set the CO2 value to 1000 ppm
        return MHZ19_OK;
    };*/

    createTemperature();

    hih8120_initialise_fake;

    runTemperature();

    ASSERT_EQ(hih8120_measure_fake.call_count, 1);
    ASSERT_EQ(hih8120_getTemperature_x10_fake.call_count, 1);
    ASSERT_EQ(xEventGroupSetBits_fake.call_count, 1);

    ASSERT_EQ(getTemperature(), 200);
}
