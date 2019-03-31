#include "errors.h"
#include "iostreamsocket.h"

IOStreamSocket::IOStreamSocket(std::istream &in, std::ostream &out) : in(in), out(out) { }

int IOStreamSocket::read(PortType port, SizeType len, void *buffer) {
    // FIXME: Handle errors
    in.read((char*)buffer, len);
    return ERR_SUCCESS;
}

int IOStreamSocket::write(PortType port, SizeType len, const void *buffer) {
    // FIXME: Handle errors
    out.write((char*)buffer, len);
    return ERR_SUCCESS;
}