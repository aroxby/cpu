#include <cstring>
#include "errors.h"
#include "staticmemory.h"

StaticMemory::StaticMemory(SizeType len) : size(len), self(new unsigned char[size]) {
}

int StaticMemory::readMemory(SizeType offset, SizeType len, void *buffer) {
    if(offset+len > size) {
        return ERR_BADRANGE;
    }
    memcpy(buffer, self+offset, len);
    return ERR_SUCCESS;
}

int StaticMemory::writeMemory(SizeType offset, SizeType len, const void *buffer) {
    if(offset+len > size) {
        return ERR_BADRANGE;
    }
    memcpy(self+offset, buffer, len);
    return ERR_SUCCESS;
}

StaticMemory::~StaticMemory() {
    delete[] self;
}