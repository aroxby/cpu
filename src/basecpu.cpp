#include "basecpu.h"
#include "errors.h"

BaseCPU::BaseCPU(const System &sys): sys(sys), running(false) {
}

void BaseCPU::stop() {
    running = false;
}

bool BaseCPU::startup() {
    bool started = false;
    if (!running) {
        running = true;
        started = true;
        reset();
        while(running) { tick(); }
    }
    return started;
}

bool BaseCPU::isRunning() {
    return running;
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

void Interruptable::signalInterrupt(Interrupt interrupt) {
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

GenericCPU::GenericCPU(const System &sys, const InstructionSet &set, Interrupt badInstruction, Interrupt badOperand) :
    BaseCPU(sys), set(set), badInstruction(badInstruction), badOperand(badOperand) {
}

void GenericCPU::tick() {
    if(!(interruptsEnabled() && serviceNextInterrupt())) {
        nextInstruction();
    }
}

bool GenericCPU::readBytes(ByteString &buffer, const void * const base, SizeType length) {
    buffer.reserve(length);
    return readMemory((SizeType)base, length, buffer.data()) == ERR_SUCCESS;
}

bool GenericCPU::readNextByte(ByteString &buffer, const void * const base) {
    bool ret = false;
    byte next;
    SizeType nextAddr = ((SizeType)base) + buffer.size();

    if(readMemory(nextAddr, 1, &next) != ERR_SUCCESS) {
        ret = false;
    } else {
        buffer.push_back(next);
        ret = true;
    }
    return ret;
}

// FIXME: Broken function.  Break up into smaller functions
void GenericCPU::nextInstruction() {
    ByteString opcode;
    int decodeRc = ERR_SUCCESS;
    const void * const instructionBase = instructionPointer();
    // FIXME: Shouldn't HAVE to be const
    const Instruction *inst;

    do {
        if(readNextByte(opcode, instructionBase))
        {
            signalInterrupt(badOperand);
        } else {
            decodeRc = set.decode(opcode, &inst);
        }
    } while(decodeRc == ERR_CONFLICT || decodeRc == ERR_INCOMPLETE);

    if(decodeRc != ERR_SUCCESS) {
        signalInterrupt(badInstruction);
    } else {
        SizeType operandLength = inst->length;
        SizeType offset = opcode.size();
        ByteString operands;
        if(!readBytes(operands, instructionBase, operandLength)) {
            signalInterrupt(badOperand);
        } else {
            // FIXME: Advance instruction pointer
            // It's import this be done before running the instruction for things like RST to work
            inst->execute(*this, operands);
        }
    }
}