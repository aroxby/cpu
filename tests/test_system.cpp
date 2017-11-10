#include <gtest/gtest.h>
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
    sys.installMemory(&mem, 10, 10);
    int iret = sys.readMemory(10, 5, dummy_buffer);
}