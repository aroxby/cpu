EXT_DIR=external

GTEST_REPO=https://github.com/google/googletest.git
GTEST_TAG=release-1.8.0
GTEST_DIR=$(EXT_DIR)/googletest
DEPENDS=$(GTEST_DIR)

GTEST_INC=$(GTEST_DIR)/googletest/include

EXT_INC=$(GTEST_INC)

SRC_INC=src

TEST_DIR=tests
TEST_SRCS=$(shell find $(TEST_DIR) -name *.cpp)
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS))
TEST_OUT=$(TEST_DIR)/tests.exe

INC=$(SRC_INC) $(EXT_INC)

CPP=g++
CPPFLAGS=$(foreach d, $(INC), -I$d) #Source: https://stackoverflow.com/a/4134861

GIT=git

.PHONY: default depend test test-tidy test-clean clean dist-clean maintainer-clean

default: all

$(GTEST_DIR):
	$(GIT) clone $(GTEST_REPO) -b $(GTEST_TAG) $@

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TEST_OUT): $(TEST_OBJS)
	$(CPP) $< -o $@

test: $(TEST_OUT)
	$<

test-tidy:
	rm -f $(TEST_OBJS)

test-clean: test-tidy
	rm -f $(TEST_OUT)

clean: test-clean

dist-clean: clean
	rm -rf $(EXT_DIR)

maintainer-clean: dist-clean
	git clean -xdf