#include <gtest/gtest.h>
#include <FreeRTOS_defs/fff.h>
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"


extern "C"{
    #include "temperature.h"
    #include "hih8120.h"
    #include "humidity.h"
}

DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC(measureHumidity);
FAKE_VOID_FUNC(createTemperature)



class Temp_test : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(measureHumidity);
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xSemaphoreTake);
		RESET_FAKE(xSemaphoreGive);
		RESET_FAKE(xTaskGetTickCount);
		RESET_FAKE(xTaskDelayUntil)
      
        FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(Temp_test, Test_createTemperature){


    createTemperature();

    ASSERT_EQ(xTaskCreate_fake.call_count, 1);
	// Check all arguments to xTaskCreate
	

}

TEST(HelloTest3, BasicAssertions) {
   
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "from test 3");
  // Expect equality.
  EXPECT_EQ(9 * 9, 81);
}