#include <gtest/gtest.h>
#include "fff/fff.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("skdhasjdjas", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest2, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "asndasllkasd");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}