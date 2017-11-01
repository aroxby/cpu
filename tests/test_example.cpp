#include <gtest/gtest.h>
#include <system.h>

class SystemTest : public testing::Test {
protected:
    System sys;
};

//Dumy test
TEST_F(SystemTest, ExampleTest) {
    EXPECT_EQ(0, 0);
}