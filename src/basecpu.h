#ifndef _INC_BASECPU_H
#define _INC_BASECPU_H

#include <queue>
#include "basetypes.h"
#include "system.h"
#include "instruction.h"

class BaseCPU {
public:
    BaseCPU(const System &sys);
    virtual void stop();
    virtual bool startup();
    bool isRunning();

    virtual void reset() = 0;
    virtual void tick() = 0;

protected:
    int readMemory(SizeType offset, SizeType len, void *data) const;
    int writeMemory(SizeType offset, SizeType len, const void *data) const;
    int readPort(PortType port, SizeType len, void *data) const;
    int writePort(PortType port, SizeType len, const void *data) const;

private:
    const System &sys;
    bool running;
};

class Interruptable {
public:
    virtual void signalInterrupt(Interrupt interrupt);
    SizeType interruptQueueLength();

protected:
    virtual bool serviceNextInterrupt();
    virtual void serviceInterrupt(Interrupt interrupt) = 0;

private:
    typedef std::queue<Interrupt> InterruptQueue;
    InterruptQueue interruptQueue;
};

class GenericCPU : public BaseCPU, public Interruptable {
public:
    GenericCPU(const System &sys, const InstructionSet &set, Interrupt badInstruction, Interrupt badOperand);
    virtual void tick();

    virtual void *instructionPointer() = 0;
    virtual bool interruptsEnabled() = 0;

protected:
    bool readBytes(ByteString &buffer, const void * const base, SizeType length);
    bool readNextByte(ByteString &buffer, const void * const base);
    int readInstruction(const void * const instructionBase, const Instruction **out);
    bool readInstructionOperands(
        const void * const instructionBase, const Instruction &instruction, ByteString &operands
    );
    bool loadNextInstruction(const Instruction **instruction, ByteString &operands);
    void nextInstruction();

    const InstructionSet &set;
    Interrupt badInstruction;
    Interrupt badOperand;
};

#endif//_INC_BASECPU_H