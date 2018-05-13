#ifndef _INC_GENERICCPU_H
#define _INC_GENERICCPU_H

#include "basetypes.h"
#include "basecpu.h"
#include "instruction.h"

class Interruptable {
public:
    virtual void serviceInterrupt(SizeType interrupt) = 0;
};

class CPUConfig {
public:
    CPUConfig(
        SizeType registerWidth, void *instructionPointer, SizeType maxInstuctionLength,
        SizeType iBadInstruction, SizeType iDoubleFault, SizeType iTripleFault
    ) : registerWidth(registerWidth), instructionPointer(instructionPointer), maxInstuctionLength(maxInstuctionLength),
        iBadInstruction(iBadInstruction), iDoubleFault(iDoubleFault), iTripleFault(iTripleFault) {
    }

    SizeType registerWidth;
    void *instructionPointer;
    SizeType maxInstuctionLength;
    SizeType iBadInstruction;
    SizeType iDoubleFault;
    SizeType iTripleFault;
};

class GenericCPU : public BaseCPU, public Interruptable {
public:
    GenericCPU(const System &sys, const InstructionSet &set, CPUConfig &config): BaseCPU(sys), set(set), config(config) { }
    virtual void tick();
    
    virtual void reset() = 0;
    virtual void serviceInterrupt(SizeType interrupt) = 0;

private:
    const InstructionSet &set;
    CPUConfig config;
};

#endif//_INC_GENERICCPU_H