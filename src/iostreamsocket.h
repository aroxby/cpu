#ifndef _INC_IOSTREAMSOCKET_H
#define _INC_IOSTREAMSOCKET_H

#include <iostream>
#include "portsocket.h"

// I/O backend interface
class IOStreamSocket: public PortSocket {
public:
    IOStreamSocket(std::istream &in, std::ostream &out);
    virtual int read(void *buffer, SizeType len, PortType port);
    virtual int write(const void *buffer, SizeType len, PortType port);

private:
    std::istream &in;
    std::ostream &out;
};

#endif//_INC_IOSTREAMSOCKET_H