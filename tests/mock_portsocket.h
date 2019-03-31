#ifndef _INC_MOCK_PORTSOCKET_H
#define _INC_MOCK_PORTSOCKET_H

#include <portsocket.h>
#include <gmock/gmock.h>

class MockPortSocket : public PortSocket {
public:
    MOCK_METHOD3(read, int(SizeType offset, SizeType len, void *buffer));
    MOCK_METHOD3(write, int(SizeType offset, SizeType len, const void *buffer));
};

typedef ::testing::StrictMock<MockPortSocket> StrictMockPortSocket;

#endif//_INC_MOCK_PORTSOCKET_H