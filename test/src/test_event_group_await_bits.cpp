// test_event_group_await_bits.cpp
#include <gtest/gtest.h>
#include <fff.h>
#include <event_groups.h>
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C" {
    #include <cotwo.h>
    #include <humidity.h>
    #include <controllerSender.h>
}

#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
// Set up the test environment
EventGroupHandle_t eventGroupHandle; // Initialize the event group handle


class EventGroupAwaitBits_awaitBits_Test : public ::testing::Test {
protected:
    void SetUp() override {
        RESET_FAKE(xEventGroupWaitBits);
        //RESET_FAKE(xEventGroupSetBits);

        FFF_RESET_HISTORY();
    }
};

TEST_F(EventGroupAwaitBits_awaitBits_Test, TestBody) {
    //Arrange
    //set bits
    EventBits_t expectedBits = BIT_0 | BIT_1 | BIT_2;

    //Act
    run_coTow();
    ASSERT_EQ(xEventGroupSetBits_fake.call_count, BIT_1);
    
    //runTaskHumidity();
    ASSERT_EQ(BIT_2, BIT_2);

    //runTemperatureTask();
    ASSERT_EQ(BIT_0, BIT_0);


    EventBits_t returnedBits = xEventGroupWaitBits_fake.return_val;
    //Assert/Expect
    //EventBits_t returnedBits = xEventGroupWaitBits_Fake.arg0_history[0]; // Get the argument from the fake function call
    ASSERT_EQ(7, expectedBits); // Check if the returned bit matches the expected value
}
