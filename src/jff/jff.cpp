#include "jff.h"

/*
If any functions are defined for a class with virtual functions then all virtual functions must be defined.
Otherwise the following linker error occurs:
undefined reference to `vtable for JFF'
JFF::JFF(const System &sys, const InstructionSet &set) : GenericCPU(sys, set, 1, 2) {
}
*/