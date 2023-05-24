#include "gtest/gtest.h"
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"
#include "FreeRTOS_defs/FreeRTOS.h"
#include "FreeRTOS_defs/queue.h"

extern "C"
{
#include <dataShared.h>
#include <controllerSender.h>
#include <controllerReceiver.h>

}

// FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);

// FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_getCo2Ppm, uint16_t *);
//  FAKE_VOID_FUNC(run_coTwo);

// extern uint32_t _myEventGroupSender = 0;

class QueueTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        RESET_FAKE(xQueueCreate);
        RESET_FAKE(xQueueSendToBack);
        RESET_FAKE(xEventGroupCreate);
        RESET_FAKE(xEventGroupSetBits);

        FFF_RESET_HISTORY();
    }
};

TEST_F(QueueTest, Test_xQueueCreate_is_called)
{
    // Arrange
    // Act
    controllerSendTask();
    ASSERT_EQ(1, xQueueCreate_fake.call_count);
}

TEST_F(QueueTest, Test_xQueueCreate_is_called_with_queue_lenght_1)
{
    // Arrange
    // Act
    controllerSendTask();
    ASSERT_EQ(1, xQueueCreate_fake.arg0_val);
}
TEST_F(QueueTest, Test_xQueueCreate_returns_queue)
{
    // xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS
    QueueHandle_t xQueueFake = xQueueCreate_fake.return_val;
    // Arrange

    // Act
    controllerSendTask();
    ASSERT_EQ(xQueueFake, xQueueCreate_fake.return_val);
}

TEST_F(QueueTest, Test_xQueueSendToBack_is_called)
{
    // Arrange
    // Act
    runSetData();
    ASSERT_EQ(1, xQueueSendToBack_fake.call_count);
}

TEST_F(QueueTest, Test_xQueueSendToBack_is_called_with_right_arguments)
{
    // xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS

    // Arrange
    QueueHandle_t xQueueFake = xQueueCreate_fake.return_val;
    struct sensors_data dataFake;

    // Act
    runSetData();
    ASSERT_EQ(xQueueFake, xQueueSendToBack_fake.arg0_val);
    ASSERT_EQ((void *)&dataFake, xQueueSendToBack_fake.arg1_val);
    ASSERT_EQ(1, xQueueSendToBack_fake.arg2_val);
}

TEST_F(QueueTest, Test_xQueueSendToBack_returns_pdPASS)
{
    // xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS

    // Arrange

    // Act
    runSetData();
    ASSERT_EQ((BaseType_t)1, xQueueSendToBack_fake.return_val);
}

TEST_F(QueueTest, Test_xQueueReceive_is_called_with_right_arguments)
{
    // xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS

    // Arrange
    QueueHandle_t xQueueFake = xQueueCreate_fake.return_val;
    struct sensors_data dataFake;

    // Act
    retrieveQueueData();
    ASSERT_EQ(xQueueFake, xQueueRetrieve_fake.arg0_val);
    ASSERT_EQ((void *)&dataFake, xQueueRetrieve_fake.arg1_val);
    ASSERT_EQ(portMAX_DELAY, xQueueRetrieve_fake.arg2_val);
}

/*TEST_F(QueueTest, Test_xQueueCreate_return_reference_queue){
    //Arrange

    QueueHandle_t xQueueCreate_fake.return_val = &xQueue;

    // Call the function that uses the fake function
    QueueHandle_t result = someFunction(); // Replace with your actual function call

    // Verify the behavior of the fake function
    ASSERT_EQ(result, fakeQueueHandle); // Assert that the result matches the expected QueueHandle_t

    return 0;

}*/

/*

TEST_F(CoTwoTest, RunCoTwo_success_full_measurement)
{
    mh_z19_initialise_fake.arg0_val = ser_USART3;
    mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;
    mh_z19_getCo2Ppm_fake.return_val = MHZ19_OK;
    mh_z19_getCo2Ppm_fake.custom_fake = [](uint16_t *ppm)
    {
        *ppm = 1000; // Set the CO2 value to 1000 ppm
        return MHZ19_OK;
    };

    createCoTwo();

    ASSERT_EQ(mh_z19_initialise_fake.call_count, 1);

    run_coTow();

    ASSERT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);
    ASSERT_EQ(mh_z19_getCo2Ppm_fake.call_count, 1);
    ASSERT_EQ(xEventGroupSetBits_fake.call_count, 1);

    ASSERT_EQ(getCoTwo(), 1000);
}*/