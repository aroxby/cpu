#ifndef _INC_JFF_H
#define _INC_JFF_H

#include "basecpu.h"

class JFF : public GenericCPU {
public:
    JFF(const System &sys, const InstructionSet &set);

    virtual void reset();
    virtual SizeType registerWidth();
    virtual void *instructionPointer();
    virtual bool interruptsEnabled();

protected:
    virtual void serviceInterrupt(Interrupt interrupt);
};

#endif//_INC_JFF_H