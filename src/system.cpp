#include <algorithm>
#include <cassert>
#include "system.h"
#include "errors.h"
#include "utils.h"

//#include <iostream>
//using namespace std;

const System::MemoryInstance System::MemoryInstance::null(NULL, 0, 0);

int System::installMemory(MemoryModule &mod, SizeType offset, SizeType size) {
    int status = ERR_SUCCESS;

    // NB: if(size <= 0 || /* overflow */offset + size < offset) {
    if(offset + size <= offset) {
        status = ERR_BADRANGE;
    } else {
        MemoryInstance overlap = resolveAtMost(offset + size - 1);
        if(overlap) {
            SizeType overlapEnd = overlap.offset + overlap.length - 1;
            if(overlapEnd >= offset) {
                status = ERR_CONFLICT;
            }
        }
    }

    if(status == ERR_SUCCESS) {
        mem.emplace(offset,  MemoryInstance(&mod, offset, size));
    }
    return status;
}

int System::removeMemory(SizeType offset) {
    MemMap::size_type erased = mem.erase(offset);
    int status = erased ? ERR_SUCCESS : ERR_BADRANGE;
    return status;
}

int System::bindPort(PortSocket &sock, PortType port) {
    throw "Not Implemented";
}

int System::releasePort(PortType port) {
    throw "Not Implemented";
}

int System::readMemory(SizeType offset, SizeType len, void *data) const {
    return memoryLoop(offset, len, data, false);
}

int System::writeMemory(SizeType offset, SizeType len, const void *data) const {
    return memoryLoop(offset, len, data, true);
}

int System::readPort(PortType port, SizeType len, void *data) const {
    throw "Not Implemented";
}

int System::writePort(PortType port, SizeType len, const void *data) const {
    throw "Not Implemented";
}

System::MemoryInstance System::resolveAtMost(SizeType address) const {
    MemoryInstance ret = MemoryInstance::null;
    auto iterator = mem.lower_bound(address);
    if(iterator->second.offset == address) {
        ret = iterator->second;
    } else {
        if(iterator != mem.cbegin()) {
            iterator--;
            ret = iterator->second;
        }
    }
    // TODO: Check if this block is required
    if(ret.offset > address) {
        ret = MemoryInstance::null;
    }
    return ret;
}

int System::memoryLoop(SizeType offset, SizeType len, const void *data, bool write) const {
    int status = ERR_SUCCESS;
    while(len > 0) {
        MemoryInstance src = resolveAtMost(offset);
        if(!src) {
            status = ERR_BADRANGE;
        } else {
            SizeType srcOff = offset - src.offset;
            //No memory module covers the requested range
            if (srcOff >= src.length) {
                status = ERR_BADRANGE;
            }
            else {
                SizeType srcLen = std::min(src.length - srcOff, len);
                if(write) {
                    status = src.mod->writeMemory(srcOff, srcLen, data);
                } else {
                    status = src.mod->readMemory(srcOff, srcLen, /* HACK */(void*)(data));
                }
                len -= srcLen;
                offset += srcLen;
                data = advancePtr(data, srcLen);
            }
        }
        if(status != ERR_SUCCESS) {
            break;
        }
    }
    return status;
}