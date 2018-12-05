#include <cassert>
#include <gtest/gtest.h>
#include <system.h>
#include <basecpu.h>

#define TEST_CLASS BaseCPUTest

class BaseTestCPU : public BaseCPU {
public:
    BaseTestCPU(int maxTicks, const System &sys) : maxTicks(maxTicks), ticks(0), resets(0), BaseCPU(sys) { }
    void reset() { resets++; }
    void tick() {
        assert(isRunning());
        ticks++;
        if(ticks>= maxTicks) {
            stop();
        }
    }

    int ticks;
    int resets;
    int maxTicks;
};

const int TEST_TICK_COUNT = 3;

class TEST_CLASS : public testing::Test {
protected:
    TEST_CLASS() : cpu(TEST_TICK_COUNT, dummySystem) { }
    BaseTestCPU cpu;
    System dummySystem;
};

TEST_F(TEST_CLASS, TestRunLoop) {
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect initial CPU running state";
    ASSERT_EQ(cpu.ticks, 0) << "Incorrect initial CPU tick count";
    ASSERT_EQ(cpu.resets, 0) << "Incorrect initial CPU reset count";
    cpu.startup();
    ASSERT_FALSE(cpu.isRunning()) << "Incorrect final CPU running state";
    ASSERT_EQ(cpu.ticks, TEST_TICK_COUNT) << "Incorrect final CPU tick count";
    ASSERT_EQ(cpu.resets, 1) << "Incorrect initial CPU reset count";
}