#include <gtest/gtest.h>
#include "fff/fff.h"
extern "C"
{
#include "temperature.h"
#include "hal.h"
}
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest2, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}


FAKE_VOID_FUNC(hal_create,uint8_t);
FAKE_VALUE_FUNC(int16_t,hal_getTemperature,uint8_t);

class Test_production : public::testing::TEST
{
  protected:
    void SetUp()override
    {
      RESET_FAKE(hal_create);
      RESET_FAKE(hal_getTemperature);
      FFF_RESET_HISTORY();
    }
    void TearDown() override
    {}
}

TEST_F(Test_production, Test_getTemperature_return_value){
  uint8_t _sensor=12;
  int16_t _temperature;

  uint8_t _hal_getTemperature_return=12;
  hal_getTemperature_fake.return_val=_hal_getTemperature_return;

  production.hal_getTemperature(12);

  ASSERRT_EQ(1,  hal_getTemperature_fake.call_count);
  ASSERRT_EQ(_sensor,hal_getTemperature_fake.arg0._val);
  ASSERRT_EQ(_hal_getTemperature_return,_temperature);


}