#include "basecpu.h"

BaseCPU::BaseCPU(const System &sys): sys(sys), running(false) {
}

void BaseCPU::stop() {
    running = false;
}

bool BaseCPU::startup() {
    bool started = false;
    if (!running) {
        started = true;
        reset();
        while(running) { tick(); }
    }
    return started;
}

int BaseCPU::readMemory(SizeType offset, SizeType len, void *data) const {
    return sys.readMemory(offset, len, data);
}

int BaseCPU::writeMemory(SizeType offset, SizeType len, const void *data) const {
    return sys.writeMemory(offset, len, data);
}

int BaseCPU::readPort(PortType port, SizeType len, void *data) const {
    return sys.readPort(port, len, data);
}

int BaseCPU::writePort(PortType port, SizeType len, const void *data) const {
    return sys.writePort(port, len, data);
}

void Interruptable::signalInterrupt(SizeType interrupt) {
    queue.push(interrupt);
}

bool Interruptable::serviceNextInterrupt() {
    bool ret = false;
    if(!queue.empty()) {
        serviceInterrupt(queue.front());
        queue.pop();
        ret = true;
    }
    return ret;
}

GenericCPU::GenericCPU(const System &sys, const InstructionSet &set, SizeType intBadInstruction) :
    BaseCPU(sys), set(set) {
}