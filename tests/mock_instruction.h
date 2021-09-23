#ifndef _INC_MOCK_INSTRUCTION_H
#define _INC_MOCK_INSTRUCTION_H

#include <gmock/gmock.h>
#include <instruction.h>

class MockInstructionSet : public InstructionSet {
public:
    // int add(const Instruction &instruction);
    MOCK_METHOD1(add, int(Instruction &instruction));
    // int remove(const ByteString &opcode);
    MOCK_METHOD1(remove, int(ByteString &opcode));
    // int decode(const ByteString &opcode, const Instruction **out) const;
    MOCK_CONST_METHOD2(decode, int(const ByteString &opcode, const Instruction **out));
    // SizeType count() const;
    MOCK_CONST_METHOD0(count, SizeType());
};

typedef ::testing::StrictMock<MockInstructionSet> StrictMockInstructionSet;

#endif//_INC_MOCK_INSTRUCTION_H