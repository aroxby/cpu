#include "jff.h"

JFF::JFF(const System &sys, const InstructionSet &set) : GenericCPU(sys, set, 1, 2) {
}

void JFF::reset() {
    throw "Not Implemented";
}

void JFF::serviceInterrupt(Interrupt interrupt) {
    throw "Not Implemented";
}