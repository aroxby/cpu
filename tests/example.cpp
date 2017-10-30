#include <system.h>
#include <gtest/gtest.h>

class SystemTest : public testing::Test {
protected:
    System sys;
};

//Dumy test
TEST_F(SystemTest, ExampleTest) {
    EXPECT_EQ(0, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}