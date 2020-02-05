#ifndef _INC_INSTRUCTION_H
#define _INC_INSTRUCTION_H

#include "basetypes.h"
#include <vector>
#include <map>

typedef std::vector<byte> ByteString;

class BaseCPU;

class Instruction {
public:
    Instruction(ByteString opcode, SizeType instructionLength);
    virtual void execute(BaseCPU &cpu, const ByteString &params) const = 0;
    const ByteString opcode;
    const SizeType length;
};

class InstructionSet {
public:
    class InstructionComparator {
    public:
        bool operator()(const Instruction &a, const Instruction &b) const {
            return a.opcode < b.opcode;
        }
    };

    int add(const Instruction &instruction);
    int remove(const ByteString &opcode);
    int decode(const ByteString &opcode, const Instruction **out) const;
    SizeType count() const;

private:
    typedef std::pair<ByteString, const Instruction &> InstructionCode;
    typedef std::map<ByteString, const Instruction &> Set;
    typedef Set::iterator Iterator;
    typedef Set::const_iterator ConstIterator;
    typedef std::pair<Iterator, bool> Insertion;

    Set set;
};

#endif//_INC_INSTRUCTION_H