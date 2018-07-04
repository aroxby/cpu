#include "jff.h"

JFF::JFF(const System &sys, const InstructionSet &set) : GenericCPU(sys, set, 1, 2) {
}

void JFF::reset() { throw; }
void JFF::serviceInterrupt(SizeType interrupt) { throw; }