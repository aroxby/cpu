#include "errors.h"
#include "iostreamsocket.h"

IOStreamSocket::IOStreamSocket(std::istream &in, std::ostream &out) : in(in), out(out) { }

int IOStreamSocket::read(PortType port, SizeType len, void *buffer) {
    in.read((char*)buffer, len);
    return in.good() ? ERR_SUCCESS : ERR_INCOMPLETE;
}

int IOStreamSocket::write(PortType port, SizeType len, const void *buffer) {
    out.write((char*)buffer, len);
    return !out.fail() ? ERR_SUCCESS : ERR_INCOMPLETE;
}