#include <sstream>
#include <gtest/gtest.h>
#include <errors.h>
#include <iostreamsocket.h>

#define TEST_CLASS IOStreamSocketTest
#define INIT_DATA ("dat_data_tho")
#define INIT_LEN (12)

class TEST_CLASS : public testing::Test {
protected:
    TEST_CLASS() : in(INIT_DATA), sock(in, out) { }
    std::stringstream in, out;
    IOStreamSocket sock;
};

TEST_F(TEST_CLASS, TestRead) {
    int iret;
    char *buffer[INIT_LEN+1] = {0};
    
    iret = sock.read(buffer, INIT_LEN, 0);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Read failed";
    ASSERT_TRUE(!memcmp(buffer, INIT_DATA, INIT_LEN)) << "Read wrong data(" << buffer << ")";
}

TEST_F(TEST_CLASS, TestWrite) {
    int iret;
    
    iret = sock.write(INIT_DATA, INIT_LEN, 0);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Read failed";
    
    const char *actual = out.str().c_str();
    ASSERT_TRUE(!memcmp(actual, INIT_DATA, INIT_LEN)) << "Wrote wrong data(" << actual << ")";
}