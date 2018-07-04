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
    virtual void signalInterrupt(SizeType interrupt);

protected:
    virtual bool serviceNextInterrupt();
    virtual void serviceInterrupt(SizeType interrupt) = 0;

private:
    typedef std::queue<SizeType> InterruptQueue;
    InterruptQueue queue;
};

class GenericCPU : public BaseCPU, public Interruptable {
public:
    GenericCPU(const System &sys, const InstructionSet &set, SizeType intBadInstruction, SizeType intBadOperand);
    virtual void tick();

    virtual SizeType registerWidth() = 0;
    virtual void *instructionPointer() = 0;
    virtual bool interruptsEnabled() = 0;

private:
    void nextInstruction();
    bool loadNextByte(ByteString &buffer, const void * const base);
    bool readBytes(ByteString &buffer, const void * const base, SizeType length);

    const InstructionSet &set;
    SizeType intBadInstruction;
    SizeType intBadOperand;
};

#endif//_INC_BASECPU_H