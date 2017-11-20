#include <cassert>
#include "system.h"
#include "errors.h"
#include "utils.h"

int System::installMemory(MemoryModule &mod, SizeType offset, SizeType size) {
    int status = ERR_SUCCESS;

    if(size <= 0) {
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
    assert(erased <= 1);
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

// Find installed module with base address <= address
System::MemoryInstance System::resolveAtMost(SizeType address) const {
    auto iterator = mem.lower_bound(address);
    if(iterator->second.offset != address) {
        if (iterator == mem.cbegin()) {
            iterator = mem.cend();
        } else if(iterator != mem.cend()) {
            iterator--;
        }
    }
    return iterator->second;
}

// Get the module to lowest address after inst
System::MemoryInstance System::next(const MemoryInstance inst) const {
    auto iterator = mem.find(inst.offset);
    if(iterator != mem.cend()) {
        iterator++;
    }
    return iterator->second;
}

int System::memoryLoop(SizeType offset, SizeType len, const void *data, bool write) const {
    int status = ERR_SUCCESS;
    while(len > 0) {
        MemoryInstance src = resolveAtMost(offset);
        if(!src) {
            status = ERR_BADRANGE;
        } else {
            SizeType srcOff = offset - src.offset;
            SizeType srcLen = src.length - srcOff;
            if(write) {
                status = src.mod->writeMemory(srcOff, srcLen, data);
            } else {
                status = src.mod->readMemory(srcOff, srcLen, /* HACK */(void*)(data));
            }
            if(status == ERR_SUCCESS)
            {
                len -= srcLen;
                offset += srcLen;
                data = advancePtr(data, srcLen);
            } else {
                break;
            }
        }
    }
    return status;
}