#include "iostreamsocket.h"

IOStreamSocket::IOStreamSocket(std::istream &in, std::ostream &out) : in(in), out(out) {
    throw "Not Implemented";
}

int IOStreamSocket::read(void *buffer, SizeType len, PortType port) {
    throw "Not Implemented";
}

int IOStreamSocket::write(const void *buffer, SizeType len, PortType port) {
    throw "Not Implemented";
}