#include <cassert>
#include "errors.h"
#include "system.h"

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
    throw "Not Implemented";
}

int System::writeMemory(SizeType offset, SizeType len, const void *data) const {
    throw "Not Implemented";
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

// Find installed module with base address >= address
System::MemoryInstance System::resolveAtLeast(SizeType address) const {
    auto iterator = mem.lower_bound(address);
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