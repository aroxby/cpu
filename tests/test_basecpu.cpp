#include <gtest/gtest.h>
#include <errors.h>
#include <basecpu.h>
#include "mock_instruction.h"
#include "mock_system.h"

#define BASECPU_TEST_CLASS BaseCPUTest
#define GENERICCPU_TEST_CLASS GenericCPUTest

class BaseTestCPU : public BaseCPU {
public:
    BaseTestCPU(int maxTicks, const System &sys) :
        maxTicks(maxTicks), resets(0), startups(0), ticks(0), BaseCPU(sys) { }

    virtual bool startup() {
        startups++;
        return BaseCPU::startup();
    }

    void reset() {
        resets++;
    }

    void tick() {
        assert(isRunning());
        ticks++;
        if(ticks>= maxTicks) {
            stop();
        }
    }

    int doReadMemory(SizeType offset, SizeType len, void *data) const {
        return BaseCPU::readMemory(offset, len, data);
    }

    int doWriteMemory(SizeType offset, SizeType len, const void *data) const {
        return BaseCPU::writeMemory(offset, len, data);
    }

    int doReadPort(PortType port, SizeType len, void *data) const {
        return BaseCPU::readPort(port, len, data);
    }

    int doWritePort(PortType port, SizeType len, const void *data) const {
        return BaseCPU::writePort(port, len, data);
    }

    int maxTicks;
    int resets;
    int startups;
    int ticks;
};

class TestInterruptable : public Interruptable {
public:
    virtual void serviceInterrupt(Interrupt interrupt) {
        serviced.push_back(interrupt);
    }

    bool _next() { return serviceNextInterrupt(); }

    std::vector<Interrupt> serviced;
};

class GenericTestCPU : public GenericCPU {
public:
    GenericTestCPU(const System &sys, const InstructionSet &set) :
        GenericCPU(sys, set, 1, 2), ip(nullptr), interruptable(true), resets(0) { }

    void *instructionPointer() { return ip; }
    bool interruptsEnabled() { return interruptable; }

    void reset() {
        resets++;
    }

    void serviceInterrupt(Interrupt interrupt) {
        interruptService.serviceInterrupt(interrupt);
    }

    bool readNextByte(ByteString &buffer, const void * const base) {
        return GenericCPU::readNextByte(buffer, base);
    }

    int readInstruction(const void * const instructionBase, const Instruction **out) {
        return GenericCPU::readInstruction(instructionBase, out);
    }

    TestInterruptable interruptService;
    void *ip;
    bool interruptable;
    int resets;
};

const int TEST_TICK_COUNT = 3;

class BASECPU_TEST_CLASS : public testing::Test {
protected:
    BASECPU_TEST_CLASS() : cpu(TEST_TICK_COUNT, dummySystem) { }
    BaseTestCPU cpu;
    TestInterruptable server;
    StrictMockSystem dummySystem;
};

TEST_F(BASECPU_TEST_CLASS, TestRunLoop) {
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect initial CPU running state";
    ASSERT_EQ(cpu.ticks, 0) << "Incorrect initial CPU tick count";
    ASSERT_EQ(cpu.resets, 0) << "Incorrect initial CPU reset count";
    bool started = cpu.startup();
    ASSERT_TRUE(started) << "CPU failed to indicate startup";
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect final CPU running state";
    ASSERT_EQ(cpu.ticks, TEST_TICK_COUNT) << "Incorrect final CPU tick count";
    ASSERT_EQ(cpu.resets, 1) << "Incorrect final CPU reset count";
    ASSERT_EQ(cpu.startups, 1) << "Incorrect final CPU startup count";
}

TEST_F(BASECPU_TEST_CLASS, TestStopStart) {
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect initial CPU running state";
    ASSERT_EQ(cpu.ticks, 0) << "Incorrect initial CPU tick count";
    ASSERT_EQ(cpu.resets, 0) << "Incorrect initial CPU reset count";
    bool started = cpu.startup();
    ASSERT_TRUE(started) << "CPU failed to indicate startup";
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect final CPU running state";
    ASSERT_EQ(cpu.ticks, TEST_TICK_COUNT) << "Incorrect final CPU tick count";
    ASSERT_EQ(cpu.resets, 1) << "Incorrect final CPU reset count";
    ASSERT_EQ(cpu.startups, 1) << "Incorrect final CPU startup count";

    started = cpu.startup();
    ASSERT_TRUE(started) << "CPU failed to indicate startup";
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect final CPU running state";
    ASSERT_EQ(cpu.ticks, TEST_TICK_COUNT + 1) << "Incorrect final CPU tick count";
    ASSERT_EQ(cpu.resets, 2) << "Incorrect final CPU reset count";
    ASSERT_EQ(cpu.startups, 2) << "Incorrect final CPU startup count";
}

TEST_F(BASECPU_TEST_CLASS, TestInterruptService) {
    ASSERT_FALSE(server._next()) << "Incorrect service of empty queue";
    server.signalInterrupt(3);
    server.signalInterrupt(5);
    ASSERT_TRUE(server._next()) << "Failed to service queue";
    ASSERT_EQ(server.serviced.size(), 1) << "Incorrect number of interrupts serviced";
    ASSERT_EQ(server.serviced.at(0), 3) << "Incorrect interrupt serviced";
    ASSERT_TRUE(server._next()) << "Failed to service queue";
    ASSERT_EQ(server.serviced.size(), 2) << "Incorrect number of interrupts serviced";
    ASSERT_EQ(server.serviced.at(1), 5) << "Incorrect interrupt serviced";
    ASSERT_FALSE(server._next()) << "Incorrect service of empty queue";
}

TEST_F(BASECPU_TEST_CLASS, TestSystemPassThru) {
    // These constants aren't important.  I just wanted to make sure calls aren't crossed
    EXPECT_CALL(dummySystem, readMemory(1, 2, 0)).WillOnce(testing::Return(ERR_SUCCESS));
    EXPECT_CALL(dummySystem, writeMemory(7, 8, 0)).WillOnce(testing::Return(ERR_SUCCESS));
    EXPECT_CALL(dummySystem, readPort(5, 6, 0)).WillOnce(testing::Return(ERR_SUCCESS));
    EXPECT_CALL(dummySystem, writePort(3, 4, 0)).WillOnce(testing::Return(ERR_SUCCESS));

    cpu.doReadMemory(1, 2, 0);
    cpu.doWriteMemory(7, 8, 0);
    cpu.doReadPort(5, 6, 0);
    cpu.doWritePort(3, 4, 0);
}

TEST_F(BASECPU_TEST_CLASS, TestReadInstruction) {
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect initial CPU running state";
    ASSERT_EQ(cpu.ticks, 0) << "Incorrect initial CPU tick count";
    ASSERT_EQ(cpu.resets, 0) << "Incorrect initial CPU reset count";
    bool started = cpu.startup();
    ASSERT_TRUE(started) << "CPU failed to indicate startup";
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect final CPU running state";
    ASSERT_EQ(cpu.ticks, TEST_TICK_COUNT) << "Incorrect final CPU tick count";
    ASSERT_EQ(cpu.resets, 1) << "Incorrect final CPU reset count";
    ASSERT_EQ(cpu.startups, 1) << "Incorrect final CPU startup count";
}

class GENERICCPU_TEST_CLASS : public testing::Test {
protected:
    GENERICCPU_TEST_CLASS() : cpu(dummySystem, dummySet) { }
    GenericTestCPU cpu;
    StrictMockSystem dummySystem;
    StrictMockInstructionSet dummySet;
};

TEST_F(GENERICCPU_TEST_CLASS, readNextByteSuccess) {
    ByteString buffer;
    const void * const baseAddr = (void*)0x1234;
    SizeType bytesToRead = 8;

    EXPECT_CALL(dummySystem, readMemory(::testing::_, 1, ::testing::_))
        .Times(bytesToRead).WillRepeatedly(testing::Return(ERR_SUCCESS));

    while(bytesToRead-- && cpu.readNextByte(buffer, baseAddr)) { }
}

TEST_F(GENERICCPU_TEST_CLASS, readNextByteFailure) {
    ByteString buffer;
    const void * const baseAddr = (void*)0x1234;
    SizeType bytesToRead = 8;
    SizeType bytesToSucceed = 5;

    EXPECT_CALL(dummySystem, readMemory(::testing::_, 1, ::testing::_))
        .WillOnce(testing::Return(ERR_BADRANGE))
        .RetiresOnSaturation();
    EXPECT_CALL(dummySystem, readMemory(::testing::_, 1, ::testing::_))
        .Times(bytesToSucceed).WillRepeatedly(testing::Return(ERR_SUCCESS))
        .RetiresOnSaturation();

    while(bytesToRead-- && cpu.readNextByte(buffer, baseAddr)) { }
}
