#include <cstring>
#include "errors.h"
#include "instruction.h"
using namespace std;

bool overlap(const ByteString &a, const ByteString &b) {
    ByteString::size_type a_len = a.size();
    ByteString::size_type b_len = b.size();
    ByteString::size_type min_len = min(a_len, b_len);
    int cmp = memcmp(a.data(), b.data(), min_len);
    return cmp == 0;
}

Instruction::Instruction(ByteString opcode, SizeType instructionLength, Callback callback) \
    : length(instructionLength), opcode(opcode), callback(callback) {
}

void Instruction::execute(CPU &cpu, const ByteString &params) const {
    return callback(cpu, *this, params);
}

int InstructionSet::add(const Instruction &instruction) {
    Insertion insert = set.insert(InstructionCode(instruction.opcode, instruction));
    Iterator pos = insert.first;
    bool success = insert.second;
    int ret = ERR_SUCCESS;
    if(success) {
        if(pos != set.begin())
        {
            Iterator prev = pos;
            prev--;
            if(overlap(pos->first, prev->first)) {
                ret = ERR_CONFLICT;
            }
        }
        if(ret == ERR_SUCCESS) {
            Iterator next = pos;
            next++;
            if(next != set.end())
            {
                if(overlap(pos->first, next->first)) {
                    ret = ERR_CONFLICT;
                }
            }
        }
    } else {
        ret = ERR_CONFLICT;
    }

    return ret;
}

int InstructionSet::remove(const ByteString &opcode) {
    Set::size_type removed = set.erase(opcode);
    if(removed > 0) {
        return ERR_SUCCESS;
    }
    return ERR_BADRANGE;
}

int InstructionSet::decode(const ByteString &opcode, const Instruction **out) const {
    ConstIterator pos = set.lower_bound(opcode);
    Set::size_type matches = 0;
    int ret = ERR_BADRANGE;
    while(pos != set.end() && overlap(opcode, pos->first)) {
        matches++;
        pos++;
    }
    if(matches >= 1) {
        ret = ERR_CONFLICT;
        if(matches == 1) {
            ret = ERR_INCOMPLETE;
            if(pos->first == opcode) {
                ret = ERR_SUCCESS;
                *out = &pos->second;
            }
        }
    }
    return ret;
}

SizeType InstructionSet::count() const {
    return set.size();
}