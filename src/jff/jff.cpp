#ifndef _INC_GENERICCPU_H
#define _INC_GENERICCPU_H

#include <queue>
#include "basetypes.h"
#include "basecpu.h"
#include "instruction.h"

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
    GenericCPU(const System &sys, const InstructionSet &set, SizeType intBadInstruction) :
        BaseCPU(sys), set(set) { }
    virtual void tick();

    virtual SizeType registerWidth() = 0;
    virtual void *instructionPointer() = 0;
    virtual bool interruptsEnabled() = 0;

private:
    const InstructionSet &set;
};

class JFF : public GenericCPU {
public:
    JFF(const System &sys, const InstructionSet &set) : GenericCPU(sys, set, 1) { }

    virtual void reset();
    virtual void signalInterrupt(SizeType interrupt);

    virtual SizeType registerWidth();
    virtual void *instructionPointer();
    virtual bool interruptsEnabled();

protected:
    virtual void serviceInterrupt(SizeType interrupt);
};

#endif//_INC_GENERICCPU_H