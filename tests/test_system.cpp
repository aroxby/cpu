#include <gtest/gtest.h>
#include <errors.h>
#include <system.h>
#include "mock_memorymodule.h"

#define TEST_CLASS SystemTest

class TEST_CLASS : public testing::Test {
protected:
    System sys;
    StrictMockMemoryModule mem;
};

TEST_F(TEST_CLASS, TestBasicMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 10, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installation failed";
}

TEST_F(TEST_CLASS, TestZeroMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 10, 0);
    ASSERT_EQ(iret, ERR_BADRANGE) << "Invalid Module install succeeded";
}

TEST_F(TEST_CLASS, TestOverlapMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 5, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installation failed";

    iret = sys.installMemory(mem, 7, 5);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Invalid address install succeeded";
}

TEST_F(TEST_CLASS, TestMultipleMemoryInstall) {
    int iret;

    iret = sys.installMemory(mem, 5, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 1 installation failed";

    iret = sys.installMemory(mem, 10, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 2 installation failed";
}

TEST_F(TEST_CLASS, TestBasicMemoryAddressTranslation) {
    int iret;
    unsigned char dummy_buffer[10];

    EXPECT_CALL(mem, readMemory(0, 5, dummy_buffer))
        .WillOnce(testing::Return(ERR_SUCCESS));

    iret = sys.installMemory(mem, 10, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installation failed";

    iret = sys.readMemory(10, 5, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Memory read failed";
}

TEST_F(TEST_CLASS, TestSpannedMemoryAddressTranslation) {
    int iret;
    unsigned char dummy_buffer[10];

    EXPECT_CALL(mem, readMemory(5, 5, dummy_buffer))
        .WillOnce(testing::Return(ERR_SUCCESS));

    EXPECT_CALL(mem, readMemory(0, 5, dummy_buffer+5))
        .WillOnce(testing::Return(ERR_SUCCESS));

    iret = sys.installMemory(mem, 0, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 1 installation failed";

    iret = sys.installMemory(mem, 10, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 2 installation failed";

    iret = sys.readMemory(5, 10, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Memory read failed";
}

TEST_F(TEST_CLASS, TestNonContinuousMemoryAddressTranslation) {
    int iret;
    unsigned char dummy_buffer[10];

    EXPECT_CALL(mem, readMemory(5, 5, dummy_buffer))
        .WillOnce(testing::Return(ERR_SUCCESS));

    iret = sys.installMemory(mem, 0, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 1 installation failed";

    iret = sys.installMemory(mem, 15, 5);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module 2 installation failed";

    iret = sys.readMemory(5, 10, dummy_buffer);
    ASSERT_EQ(iret, ERR_BADRANGE) << "Bad memory read succeeded";
}