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
    interruptQueue.push(interrupt);
}

SizeType Interruptable::interruptQueueLength() {
    return interruptQueue.size();
}

bool Interruptable::serviceNextInterrupt() {
    bool ret = false;
    if(!interruptQueue.empty()) {
        serviceInterrupt(interruptQueue.front());
        interruptQueue.pop();
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

int GenericCPU::readInstruction(const void * const instructionBase, const Instruction **out) {
    ByteString opcode;
    bool memoryRc;
    int decodeRc;

    do {
        memoryRc = readNextByte(opcode, instructionBase);
        if(memoryRc) {
            decodeRc = set.decode(opcode, out);
        }
    } while(
        memoryRc && (decodeRc == ERR_CONFLICT || decodeRc == ERR_INCOMPLETE)
    );

    if(!memoryRc) {
        return ERR_CONFLICT;
    }

    if(decodeRc != ERR_SUCCESS) {
        return ERR_BADRANGE;
    } else {
        return ERR_SUCCESS;
    }
}

bool GenericCPU::readInstructionOperands(
    const void * const instructionBase,
    const Instruction &instruction,
    ByteString &operands
) {
    const void * const operandBase = (void*)((char*)instructionBase + instruction.opcode.size());
    return readBytes(operands, operandBase, instruction.length);
}

void GenericCPU::loadNextInstruction(const Instruction **instruction, ByteString &operands) {
    const void * const instructionBase = instructionPointer();
    int instructionRc = readInstruction(instructionBase, instruction);
    if(instructionRc == ERR_SUCCESS) {
        bool operandRc = readInstructionOperands(instructionBase, **instruction, operands);
        if(!operandRc) {
            signalInterrupt(badOperand);
        }
    } else if(instructionRc == ERR_CONFLICT) {
        signalInterrupt(badOperand);
    } else if(instructionRc == ERR_BADRANGE) {
        signalInterrupt(badInstruction);
    }
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