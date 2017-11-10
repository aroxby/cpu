#include "system.h"

int System::installMemory(MemoryModule *mod, SizeType offset, SizeType size){
    throw "Not Implemented";
}

int System::removeMemory(SizeType offset){
    throw "Not Implemented";
}

int System::bindPort(PortSocket *sock, PortType port){
    throw "Not Implemented";
}

int System::releasePort(PortType port){
    throw "Not Implemented";
}

int System::readMemory(SizeType offset, SizeType len, void *data){
    throw "Not Implemented";
}

int System::writeMemory(SizeType offset, SizeType len, const void *data){
    throw "Not Implemented";
}

int System::readPort(PortType port, SizeType len, void *data){
    throw "Not Implemented";
}

int System::writePort(PortType port, SizeType len, const void *data){
    throw "Not Implemented";
}
