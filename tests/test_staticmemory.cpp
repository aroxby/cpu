#include <gtest/gtest.h>
#include <errors.h>
#include <staticmemory.h>

#define TEST_CLASS StaticMemoryTest

class TEST_CLASS : public testing::Test {
protected:
    TEST_CLASS() : mem(10) { }
    StaticMemory mem;
};

TEST_F(TEST_CLASS, TestReadWrite) {
    int iret;
    unsigned char write_buffer[10];
    unsigned char read_buffer[10];
    
    memcpy(write_buffer, "0123456789", 10);
    iret = mem.writeMemory(0, 10, write_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Write failed";

    memset(read_buffer, 0, 10);
    iret = mem.readMemory(0, 10, read_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Read failed";
    ASSERT_TRUE(!memcmp(read_buffer, "0123456789", 10)) << "Read wrong data(" << read_buffer << ")";
    
    memset(read_buffer, 0, 10);
    iret = mem.readMemory(2, 6, read_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Read failed";
    ASSERT_TRUE(!memcmp(read_buffer, "234567", 6)) << "Read wrong data(" << read_buffer << ")";
}

TEST_F(TEST_CLASS, TestAddressRange) {
    int iret;
    unsigned char dummy_buffer[10];
    
    iret = mem.readMemory(0, 10, dummy_buffer);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Valid read failed";

    iret = mem.readMemory(1, 10, dummy_buffer);
    ASSERT_EQ(iret, ERR_BADRANGE) << "Invalid read succeeded";
}