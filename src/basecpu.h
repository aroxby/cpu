#ifndef _INC_BASECPU_H
#define _INC_BASECPU_H

#include "basetypes.h"
#include "system.h"

class BaseCPU {
public:
    BaseCPU(const System &sys): sys(sys), running(false) { }
    virtual void stop() { running = false; }
    virtual bool startup();

    virtual void reset() = 0;
    virtual void tick() = 0;

private:
    const System &sys;
    bool running;

    int readMemory(SizeType offset, SizeType len, void *data) const {
        return sys.readMemory(offset, len, data);
    }

    int writeMemory(SizeType offset, SizeType len, const void *data) const {
        return sys.writeMemory(offset, len, data);
    }

    int readPort(PortType port, SizeType len, void *data) const {
        return sys.readPort(port, len, data);
    }

    int writePort(PortType port, SizeType len, const void *data) const {
        return sys.writePort(port, len, data);
    }
};

#endif//_INC_BASECPU_H