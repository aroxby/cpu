#ifndef _INC_PORTSOCKET_H
#define _INC_PORTSOCKET_H

typedef unsigned long long PortType;

// I/O backend interface
class PortSocket {
// All methods return 0 on success and non-zero on error
public:
    // Called when data should read into buffer
    virtual int read(void *buffer, SizeType len, PortType port) = 0;

    // Called when data should be written from buffer
    virtual int write(const void *buffer, SizeType len, PortType port) = 0;

    // Allow for proper polymorphic destruction
    virtual ~MemoryModule() { }
};

#endif//_INC_PORTSOCKET_H