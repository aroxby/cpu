EXT_DIR=external

GTEST_REPO=https://github.com/google/googletest.git
# Ideally, this would be a release tag but mingw compilation requires unreleased features
GTEST_TAG=master
GTEST_BASE_DIR=$(EXT_DIR)/googletest
GTEST_DIR=$(GTEST_BASE_DIR)/googletest
GTEST_INC=$(GTEST_DIR)/include
GTEST_CPPFLAGS=-isystem $(GTEST_INC) -I$(GTEST_DIR)
GTEST_SRCS=$(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJS=$(subst .cc,.o,$(GTEST_SRCS))
GTEST_OUT=$(GTEST_DIR)/libgtest.a

DEPENDS=$(GTEST_OUT)

EXT_INC=$(GTEST_INC)
SRC_INC=src
INC=$(SRC_INC) $(EXT_INC)
CPPFLAGS=$(foreach d, $(INC), -I$d) # Source: https://stackoverflow.com/a/4134861

AR=ar
GIT=git
CPP=g++

TEST_DIR=tests
TEST_SRCS=$(shell find $(TEST_DIR) -name *.cpp)
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS))
TEST_OUT=$(TEST_DIR)/tests.exe
TEST_CPPFLAGS=$(CPPFLAGS) -L $(GTEST_DIR) -lgtest
TEST_LD_FLAGS=$(DEPENDS)

.PHONY: default depend test test-tidy test-clean clean dist-clean maintainer-clean

default: all

$(GTEST_BASE_DIR):
	$(GIT) clone $(GTEST_REPO) -b $(GTEST_TAG) $@

$(GTEST_SRCS): %.cc: $(GTEST_BASE_DIR)

$(GTEST_OBJS): %.o: %.cc
	$(CPP) $(GTEST_CPPFLAGS) -c $< -o $@

$(GTEST_OUT): $(GTEST_OBJS)
	$(AR) -rv $@ $?

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TEST_OUT): $(TEST_OBJS)
	$(CPP) $< -o $@ $(TEST_CPPFLAGS)

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