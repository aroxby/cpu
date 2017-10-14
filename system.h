#ifndef _INC_SYSTEM_H
#define _INC_SYSTEM_H

#include "memorymodule.h"
#include "portsocket.h"

// This class serves as a unified bus for memory and I/O ports
class System {
public:
    int installMemory(MemoryModule *mod, const void *user_data, SizeType size);
    int removeMemory(MemoryModule *mod, const void *user_data);

    int bindPort(PortSocket *sock, PortType port, const void *user_data);
    int releasePort(PortSocket *sock, PortType port, const void *user_data);

    int readMemory(SizeType addr, SizeType len, void *data);
    int writeMemory(SizeType addr, SizeType len, const void *data);
    int readPort(PortType port, SizeType len, void *data);
    int writePort(PortType port, SizeType len, const void *data);
};

#endif//_INC_SYSTEM_H