#include "errors.h"
#include "iostreamsocket.h"

IOStreamSocket::IOStreamSocket(std::istream &in, std::ostream &out) : in(in), out(out) { }

int IOStreamSocket::read(void *buffer, SizeType len, PortType port) {
    // FIXME: Handle errors
    in.read((char*)buffer, len);
    return ERR_SUCCESS;
}

int IOStreamSocket::write(const void *buffer, SizeType len, PortType port) {
    // FIXME: Handle errors
    out.write((char*)buffer, len);
    return ERR_SUCCESS;
}