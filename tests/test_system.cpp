#include <gtest/gtest.h>
#include <errors.h>
#include <system.h>
#include "mock_memorymodule.h"

//#include <iostream>
//using namespace std;

class SystemTest : public testing::Test {
protected:
    System sys;
    StrictMockMemoryModule mem;
};

TEST_F(SystemTest, TestBasicMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 10, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installion failed";
}

TEST_F(SystemTest, TestZeroMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 10, 0);
    ASSERT_EQ(iret, ERR_BADRANGE) << "Invalid Module install succeeded";
}

TEST_F(SystemTest, TestOverlapMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 5, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installion failed";

    iret = sys.installMemory(mem, 7, 5);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Invalid address install succeeded";
}

TEST_F(SystemTest, TestMultipleMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 5, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 1 installion failed";

    iret = sys.installMemory(mem, 10, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 2 installion failed";
}

TEST_F(SystemTest, TestBasicMemoryAddressTranslation) {
    int iret;
    unsigned char dummy_buffer[10];

    EXPECT_CALL(mem, readMemory(0, 5, dummy_buffer))
        .WillOnce(testing::Return(ERR_SUCCESS));

    iret = sys.installMemory(mem, 10, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installion failed";

    iret = sys.readMemory(10, 5, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Memory read failed";
}

TEST_F(SystemTest, TestSpannedMemoryAddressTranslation) {
    int iret;
    unsigned char dummy_buffer[10];

    EXPECT_CALL(mem, readMemory(5, 5, dummy_buffer))
        .WillOnce(testing::Return(ERR_SUCCESS));

    EXPECT_CALL(mem, readMemory(0, 5, dummy_buffer+5))
        .WillOnce(testing::Return(ERR_SUCCESS));

    iret = sys.installMemory(mem, 0, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 1 installion failed";

    iret = sys.installMemory(mem, 10, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 2 installion failed";

    iret = sys.readMemory(5, 10, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Memory read failed";
}