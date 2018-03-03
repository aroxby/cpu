#ifndef _INC_INSTRUCTION_H
#define _INC_INSTRUCTION_H

#include "basetypes.h"
#include <vector>
#include <map>

typedef std::vector<byte> ByteString;

class CPU;

class Instruction {
public:
    typedef void(*Callback)(CPU &cpu, const Instruction &instruction, const ByteString &params);
    Instruction(ByteString opcode, SizeType instructionLength, Callback callback);
    void execute(CPU &cpu, const ByteString &params) const;
    const ByteString opcode;
    const SizeType length;
    const Callback callback;
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
    int get(const ByteString &opcode) const;
    int decode(const ByteString &opcode, const Instruction **out) const;
    SizeType count() const;

private:
    typedef std::pair<ByteString, Instruction> InstructionCode;
    typedef std::map<ByteString, Instruction> Set;
    typedef Set::iterator Iterator;
    typedef Set::const_iterator ConstIterator;
    typedef std::pair<Iterator, bool> Insertion;
    Set set;
};

#endif//_INC_INSTRUCTION_H