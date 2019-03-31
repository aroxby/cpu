#ifndef _INC_PORTSOCKET_H
#define _INC_PORTSOCKET_H

#include "basetypes.h"

typedef unsigned long long PortType;

// I/O backend interface
class PortSocket {
// All methods return 0 on success and non-zero on error
public:
    // Called when data should read into buffer
    virtual int read(PortType port, SizeType len, void *buffer) = 0;

    // Called when data should be written from buffer
    virtual int write(PortType port, SizeType len, const void *buffer) = 0;

    // Allow for proper polymorphic destruction
    virtual ~PortSocket();
};

#endif//_INC_PORTSOCKET_H