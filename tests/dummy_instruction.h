#ifndef _INC_DUMMY_INSTRUCTION_H
#define _INC_DUMMY_INSTRUCTION_H

class DummyInstruction : public Instruction {
public:
    DummyInstruction(ByteString opcode, SizeType instructionLength)
        : Instruction(opcode, instructionLength) {
    }

    virtual void execute(BaseCPU &cpu, const ByteString &params) const {
    }
};

#endif//_INC_DUMMY_INSTRUCTION_H