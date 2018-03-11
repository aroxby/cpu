#include <gtest/gtest.h>
#include <errors.h>
#include <instruction.h>

#define TEST_CLASS InstructionSetTest

class TEST_CLASS : public testing::Test {
protected:
    TEST_CLASS();
    InstructionSet set;
    Instruction low, high;
};

TEST_CLASS::TEST_CLASS() : low({'a', 'b'}, 0, NULL), high({'a', 'b', 'c'}, 0, NULL) {
}

TEST_F(TEST_CLASS, TestAddPreOverlap) {
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(high);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Added overlapping instruction";

    iret = set.count();
    ASSERT_EQ(iret, 1) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddPostOverlap) {
    int iret;

    iret = set.add(high);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Added overlapping instruction";

    iret = set.count();
    ASSERT_EQ(iret, 1) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddExactOverlap) {
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Added duplicate instruction";

    iret = set.count();
    ASSERT_EQ(iret, 1) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddSuccess) {
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.count();
    ASSERT_EQ(iret, 1) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddDiffernt) {
    Instruction other({'x'}, 0, NULL);
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(other);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.count();
    ASSERT_EQ(iret, 2) << "Incorrect instruction count";
}