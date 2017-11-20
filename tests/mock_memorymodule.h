#ifndef _INC_MOCK_MEMORYMODULE_H
#define _INC_MOCK_MEMORYMODULE_H

#include <memorymodule.h>
#include <gmock/gmock.h>

class MockMemoryModule : public MemoryModule {
public:
    MOCK_METHOD3(readMemory, int(SizeType offset, SizeType len, void *buffer));
    MOCK_METHOD3(writeMemory, int(SizeType offset, SizeType len, const void *buffer));
};

typedef ::testing::StrictMock<MockMemoryModule> StrictMockMemoryModule;

#endif//_INC_MOCK_MEMORYMODULE_H