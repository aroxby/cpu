#include <system.h>
#include <gtest/gtest.h>

class SystemTest : public testing::Test {
protected:
    System sys;
};

// Tests the default c'tor.
TEST_F(SystemTest, ExampleTest) {
    EXPECT_EQ(0, 0);
}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}