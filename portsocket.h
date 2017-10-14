#ifndef _INC_PORTSOCKET_H
#define _INC_PORTSOCKET_H

typedef unsigned long long PortType;

// I/O backend interface
class PortSocket {
public:
    virtual int read(void *buffer, SizeType len, void *user_data) = 0;
    virtual int write(const void *buffer, SizeType len, void *user_data) = 0;
};

#endif//_INC_PORTSOCKET_H