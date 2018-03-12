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

TEST_CLASS::TEST_CLASS() : low({'a', 'b'}, 0, nullptr), high({'a', 'b', 'c'}, 0, nullptr) {
    Instruction seed1({'z', '1'}, 0, nullptr);
    Instruction seed2({'z', '2', 'a'}, 0, nullptr);
    set.add(seed1);
    set.add(seed2);
    int iret = set.count();
    EXPECT_EQ(iret, 2) << "Incorrect instruction count during setup";
}

TEST_F(TEST_CLASS, TestAddPreOverlap) {
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(high);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Added overlapping instruction";

    iret = set.count();
    ASSERT_EQ(iret, 3) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddPostOverlap) {
    int iret;

    iret = set.add(high);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Added overlapping instruction";

    iret = set.count();
    ASSERT_EQ(iret, 3) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddExactOverlap) {
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Added duplicate instruction";

    iret = set.count();
    ASSERT_EQ(iret, 3) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddSuccess) {
    int iret;

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.count();
    ASSERT_EQ(iret, 3) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestAddDiffernt) {
    int iret;
    Instruction other({'v'}, 0, nullptr);

    iret = set.add(low);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.add(other);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Failed to add instruction";

    iret = set.count();
    ASSERT_EQ(iret, 4) << "Incorrect instruction count";
}

TEST_F(TEST_CLASS, TestDecodeMultipleMatches) {
    int iret;
    const Instruction *out;

    out = nullptr;
    iret = set.decode({'z'}, &out);
    ASSERT_EQ(iret, ERR_CONFLICT) << "Failed to match all instructions";
    ASSERT_EQ(out, nullptr) << "Incorrectly Returned instruction data";
}

TEST_F(TEST_CLASS, TestDecodeNoMatches) {
    int iret;
    const Instruction *out;

    out = nullptr;
    iret = set.decode({'y'}, &out);
    ASSERT_EQ(iret, ERR_BADRANGE) << "Incorrectly matched instructions";
    ASSERT_EQ(out, nullptr) << "Incorrectly Returned instruction data";
}

TEST_F(TEST_CLASS, TestDecodeSingleMatch) {
    int iret;
    const Instruction *out;

    out = nullptr;
    iret = set.decode({'z', '2'}, &out);
    ASSERT_EQ(iret, ERR_INCOMPLETE) << "Incorrectly matched instructions";
    ASSERT_EQ(out, nullptr) << "Incorrectly Returned instruction data";
}

TEST_F(TEST_CLASS, TestDecodeExactMatch) {
    int iret;
    const Instruction *out;

    out = nullptr;
    iret = set.decode({'z', '2', 'a'}, &out);
    ASSERT_EQ(iret, ERR_SUCCESS) << "Incorrectly matched instructions";
    ASSERT_NE(out, nullptr) << "Incorrectly Returned null instruction data";
}