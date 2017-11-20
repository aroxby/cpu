EXT_DIR=external

GTEST_REPO=https://github.com/google/googletest.git
# Ideally, this would be a release tag but mingw compilation requires unreleased features
GTEST_TAG=master
GTEST_BASE_DIR=$(EXT_DIR)/googletest
GTEST_DIR=$(GTEST_BASE_DIR)/googletest
GMOCK_DIR=$(GTEST_BASE_DIR)/googlemock

GMOCK_INC=$(GMOCK_DIR)/include
GTEST_INC=$(GTEST_DIR)/include
GTEST_CPPFLAGS=-isystem $(GTEST_INC) -isystem $(GMOCK_INC) -I$(GTEST_DIR) -I$(GMOCK_DIR)

GTEST_SRCS=$(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJS=$(subst .cc,.o,$(GTEST_SRCS))

GMOCK_SRCS=$(GMOCK_DIR)/src/gmock-all.cc
GMOCK_OBJS=$(subst .cc,.o,$(GMOCK_SRCS))

GTEST_OUT=$(GTEST_DIR)/libgtest.a

DEPENDS=$(GTEST_OUT)

EXT_INC=$(GTEST_INC) $(GMOCK_INC)
SRC_INC=src
INC=$(SRC_INC) $(EXT_INC)
CPPFLAGS=$(foreach d, $(INC), -I$d) # Source: https://stackoverflow.com/a/4134861

TEST_DIR=tests
TEST_SRCS=$(shell find $(TEST_DIR) -name *.cpp)
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS))
TEST_OUT=$(TEST_DIR)/tests.exe
TEST_CPPFLAGS=$(CPPFLAGS) -L $(GTEST_DIR) -lgtest
TEST_LD_FLAGS=$(DEPENDS)

SRC_DIR=src
SRCS=$(shell find $(SRC_DIR) -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

AR=ar
GIT=git
CPP=g++

.PHONY: default depend test test-tidy test-clean clean dist-clean maintainer-clean

default: all

$(GTEST_BASE_DIR):
	$(GIT) clone $(GTEST_REPO) -b $(GTEST_TAG) $@

$(GTEST_SRCS): %.cc: $(GTEST_BASE_DIR)

$(GMOCK_SRCS): %.cc: $(GTEST_BASE_DIR)

$(GTEST_OBJS): %.o: %.cc
	$(CPP) $(GTEST_CPPFLAGS) -c $< -o $@

$(GMOCK_OBJS): %.o: %.cc
	$(CPP) $(GTEST_CPPFLAGS) -c $< -o $@

$(GTEST_OUT): $(GTEST_OBJS) $(GMOCK_OBJS)
	$(AR) -rv $@ $(GTEST_OBJS) $(GMOCK_OBJS)

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TEST_OUT): $(TEST_OBJS) $(OBJS)
	$(CPP) $(TEST_OBJS) $(OBJS) -o $@ $(TEST_CPPFLAGS)

test: $(TEST_OUT)
	$<

test-tidy:
	rm -f $(TEST_OBJS)

test-clean: test-tidy
	rm -f $(TEST_OUT)

tidy: test-tidy
	rm -f $(OBJS)

clean: tidy test-clean

dist-clean: clean
	rm -rf $(EXT_DIR)

maintainer-clean: dist-clean
	git clean -xdf