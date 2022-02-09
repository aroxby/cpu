#include "system.h"
#include "errors.h"
#include "utils.h"

const System::MemoryInstance System::MemoryInstance::null(nullptr, 0, 0);

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
    auto inserted = ports.insert({port, &sock});
    int status = inserted.second ? ERR_SUCCESS : ERR_CONFLICT;
    return status;
}

int System::releasePort(PortType port) {
    PortMap::size_type erased = ports.erase(port);
    int status = erased ? ERR_SUCCESS : ERR_BADRANGE;
    return status;
}

int System::readMemory(SizeType offset, SizeType len, void *data) const {
    return memoryLoop(offset, len, data, false);
}

int System::writeMemory(SizeType offset, SizeType len, const void *data) const {
    return memoryLoop(offset, len, data, true);
}

int System::readPort(PortType port, SizeType len, void *data) const {
    int status;
    PortSocket *sock = getSocket(port);
    if(!sock) {
        status = ERR_BADRANGE;
    } else {
        status = sock->read(port, len, data);
    }
    return status;
}

int System::writePort(PortType port, SizeType len, const void *data) const {
    int status;
    PortSocket *sock = getSocket(port);
    if(!sock) {
        status = ERR_BADRANGE;
    } else {
        status = sock->write(port, len, data);
    }
    return status;
}

PortSocket *System::getSocket(PortType port) const {
    auto iterator = ports.find(port);
    PortSocket *sock;
    if(iterator == ports.end()) {
        sock = nullptr;
    } else {
        sock = iterator->second;
    }
    return sock;
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