#ifndef _INC_JFF_H
#define _INC_JFF_H

#include "basecpu.h"

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

#endif//_INC_JFF_H