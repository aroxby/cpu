#include "basecpu.h"
#include "errors.h"
#include "utils.h"

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
    /*
    This looks like it only services a single interrupt but remember that this
    loop also executes the handler.  Interrupts should be disabled prior to
    jumping into handler code.
    */
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
    const void * const operandBase = advancePtr(instructionBase, instruction.opcode.size());
    return readBytes(operands, operandBase, instruction.length);
}

bool GenericCPU::loadNextInstruction(const Instruction **instruction, ByteString &operands) {
    bool instructionLoaded = false;
    const void * const instructionBase = getInstructionPointer();
    int instructionRc = readInstruction(instructionBase, instruction);
    if(instructionRc == ERR_SUCCESS) {
        bool operandRc = readInstructionOperands(instructionBase, **instruction, operands);
        if(!operandRc) {
            signalInterrupt(badOperand);
        } else {
            instructionLoaded = true;
        }
    } else if(instructionRc == ERR_CONFLICT) {
        signalInterrupt(badOperand);
    } else if(instructionRc == ERR_BADRANGE) {
        signalInterrupt(badInstruction);
    }

    return instructionLoaded;
}

void GenericCPU::nextInstruction() {
    const Instruction *instruction;
    ByteString operands;

    if(loadNextInstruction(&instruction, operands)) {
        SizeType totalInstuctionLength = instruction->opcode.size() + instruction->length;
        void *oldInstructionPointer = getInstructionPointer();
        setInstructionPointer(advancePtr(oldInstructionPointer, totalInstuctionLength));
        instruction->execute(*this, operands);
    } else { /* Interrupts should handle this case */ }
}