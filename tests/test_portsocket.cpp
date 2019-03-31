#include <sstream>
#include <gtest/gtest.h>
#include "portsocket.h"

#define TEST_CLASS PortSocketTest

class TEST_CLASS : public testing::Test {
protected:
    class DummySocket : PortSocket {
    public:
        DummySocket(bool *sentinel) : sentinel(sentinel) { *sentinel = true; }
        int read(PortType port, SizeType len, void *buffer) { }
        int write(PortType port, SizeType len, const void *buffer) { }
        ~DummySocket() { *sentinel = false; }
    private:
        bool *sentinel;
    };
};

TEST_F(TEST_CLASS, TestConstructDestruct) {
    bool sentinel = false;
    {
        DummySocket sock(&sentinel);
        ASSERT_EQ(sentinel, true) << "Failed to construct instance";
    }
    ASSERT_EQ(sentinel, false) << "Failed to destruct instance";
}