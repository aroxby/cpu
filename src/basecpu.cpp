#include "basecpu.h"

bool BaseCPU::startup() {
    bool started = false;
    if (!running) {
        started = true;
        reset();
        while(running) { tick(); }
    }
    return started;
}