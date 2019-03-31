#ifndef _INC_IOSTREAMSOCKET_H
#define _INC_IOSTREAMSOCKET_H

#include <iostream>
#include "portsocket.h"

// I/O backend interface
class IOStreamSocket: public PortSocket {
public:
    IOStreamSocket(std::istream &in, std::ostream &out);
    virtual int read(PortType port, SizeType len, void *buffer);
    virtual int write(PortType port, SizeType len, const void *buffer);

private:
    std::istream &in;
    std::ostream &out;
};

#endif//_INC_IOSTREAMSOCKET_H