EXT_DIR=external
GTEST_DIR=$(EXT_DIR)/googletest
DEPENDS=$(GTEST_DIR)

GTEST_INC=-I$(GTEST_DIR)/googletest/include
EXT_INC=$(GTEST_INC)
INC=-I. $(EXT_INC)

CPP=g++
CPPFLAGS=$(INC)

TEST_DIR=tests
TEST_SRCS=$(shell find $(TEST_DIR) -name *.cpp)
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS))
TEST_OUT=$(TEST_DIR)/tests.exe

.PHONY: default depend test test-clean clean dist-clean

default: all

$(GTEST_DIR):
	git clone https://github.com/google/googletest.git $@

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TEST_OUT): $(TEST_OBJS)
	$(CPP) -o $(TEST_OUT) $<

test: $(TEST_OUT)
	$<

test-clean:
	rm -f $(TEST_OBJS)

clean: test-clean

dist-clean: clean
	rm -rf $(EXT_DIR)

maintainer-clean: dist-clean
	git clean -xdf