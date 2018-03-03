#ifndef _INC_INSTRUCTION_H
#define _INC_INSTRUCTION_H

#include "basetypes.h"
#include <vector>
#include <set>

class CPU;

class Instruction {
public:
    typedef void(*Callback)(CPU &cpu, const Instruction &instruction, const std::vector<byte> &params);
    Instruction(std::vector<byte> opcode, SizeType instructionLength, Callback callback);
    void execute(CPU &cpu, const std::vector<byte> &params) const;
    const std::vector<byte> opcode;
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
    int remove(const std::vector<byte> &opcode);
    int get(const std::vector<byte> &opcode) const;
    int decode(const std::vector<byte> &opcode, const Instruction **out) const;
    SizeType count() const;

private:
    typedef std::set<Instruction, InstructionComparator> Set;
    typedef Set::iterator Iterator;
    typedef std::pair<Iterator, bool> Insertion;
    Set set;
};

#endif//_INC_INSTRUCTION_H