#include <gtest/gtest.h>
#include <errors.h>
#include <system.h>
#include "mock_memorymodule.h"

class SystemTest : public testing::Test {
protected:
    System sys;
    StrictMockMemoryModule mem;
};

//Dumy test
TEST_F(SystemTest, TestMemoryAddressTranslation) {
    int iret;
    unsigned char dummy_buffer[10];

    EXPECT_CALL(mem, readMemory(0, 5, dummy_buffer))
        .WillOnce(testing::Return(ERR_SUCCESS));

    iret = sys.installMemory(mem, 10, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installion failed";

    iret = sys.readMemory(10, 5, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Memory read failed";
}