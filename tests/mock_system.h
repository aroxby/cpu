#ifndef _INC_MOCK_SYSTEM_H
#define _INC_MOCK_SYSTEM_H

#include <gmock/gmock.h>
#include <system.h>

class MockSystem : public System {
public:
    MOCK_METHOD3(installMemory, int(MemoryModule &mod, SizeType offset, SizeType size));
    MOCK_METHOD1(removeMemory, int(SizeType offset));
    MOCK_METHOD2(bindPort, int(PortSocket &sock, PortType port));
    MOCK_METHOD1(releasePort, int(PortType port));
    MOCK_CONST_METHOD3(readMemory, int(SizeType offset, SizeType len, void *data));
    MOCK_CONST_METHOD3(writeMemory, int(SizeType offset, SizeType len, const void *data));
    MOCK_CONST_METHOD3(readPort, int(PortType port, SizeType len, void *data));
    MOCK_CONST_METHOD3(writePort, int(PortType port, SizeType len, const void *data));
};

typedef ::testing::StrictMock<MockSystem> StrictMockSystem;

#endif//_INC_MOCK_SYSTEM_H