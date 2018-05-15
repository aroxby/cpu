#ifndef _INC_BASECPU_H
#define _INC_BASECPU_H

#include <queue>
#include "basetypes.h"
#include "system.h"
#include "instruction.h"

class BaseCPU {
public:
    BaseCPU(const System &sys): sys(sys), running(false) { }
    virtual void stop() { running = false; }
    virtual bool startup();

    virtual void reset() = 0;
    virtual void tick() = 0;

private:
    const System &sys;
    bool running;

    int readMemory(SizeType offset, SizeType len, void *data) const {
        return sys.readMemory(offset, len, data);
    }

    int writeMemory(SizeType offset, SizeType len, const void *data) const {
        return sys.writeMemory(offset, len, data);
    }

    int readPort(PortType port, SizeType len, void *data) const {
        return sys.readPort(port, len, data);
    }

    int writePort(PortType port, SizeType len, const void *data) const {
        return sys.writePort(port, len, data);
    }
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
    GenericCPU(const System &sys, const InstructionSet &set, SizeType intBadInstruction) :
        BaseCPU(sys), set(set) { }
    virtual void tick();

    virtual SizeType registerWidth() = 0;
    virtual void *instructionPointer() = 0;
    virtual bool interruptsEnabled() = 0;

private:
    const InstructionSet &set;
};

#endif//_INC_BASECPU_H