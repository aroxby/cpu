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
    unsigned char dummy_buffer[10];
    int iret;

    iret = sys.installMemory(mem, 10, 10);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Module installion failed";

    iret = sys.readMemory(10, 5, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Memory read failed";
}