#ifndef _INC_SYSTEM_H
#define _INC_SYSTEM_H

#include <map>
#include "memorymodule.h"
#include "portsocket.h"

// This class serves as a unified bus for memory and I/O ports
class System {
// All methods return 0 on success and non-zero on error
public:
    // Installs mod to support the memory at offset for size
    int installMemory(MemoryModule &mod, SizeType offset, SizeType size);

    // Removes the MemoryModule that supports the offset
    int removeMemory(SizeType offset);

    //Installs sock as the read/write interface on port
    int bindPort(PortSocket &sock, PortType port);

    // Removes the PortSocket on port
    int releasePort(PortType port);

    // Read the memory from the specified range (must be continuous) into data
    int readMemory(SizeType offset, SizeType len, void *data);

    // Write data into the specified memory range (must be continuous)
    int writeMemory(SizeType offset, SizeType len, const void *data);

    // Read data from port
    int readPort(PortType port, SizeType len, void *data);

    // Write data to port
    int writePort(PortType port, SizeType len, const void *data);

private:
    class MemoryInstance {
    public:
        SizeType offset;
        SizeType length;
        MemoryModule &mod;

        MemoryInstance(MemoryModule &mod, SizeType offset, SizeType length) :
            mod(mod), offset(offset), length(length) {
        }
    };

    typedef std::map<SizeType, const MemoryInstance> MemMap;
    typedef std::map<PortType, PortSocket*> PortMap;

    MemMap mem;
    PortMap ports;
};

#endif//_INC_SYSTEM_H