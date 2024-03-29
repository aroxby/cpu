EXT_DIR=external

GTEST_REPO=https://github.com/google/googletest.git
GTEST_TAG=release-1.8.1
GTEST_BASE_DIR=$(EXT_DIR)/googletest
GTEST_DIR=$(GTEST_BASE_DIR)/googletest
GMOCK_DIR=$(GTEST_BASE_DIR)/googlemock

GTEST_NO_PTHREAD=-DGTEST_HAS_PTHREAD=0
GMOCK_INC=$(GMOCK_DIR)/include
GTEST_INC=$(GTEST_DIR)/include
GTEST_CPPFLAGS=$(GTEST_NO_PTHREAD) -isystem $(GTEST_INC) -isystem $(GMOCK_INC) -I$(GTEST_DIR) -I$(GMOCK_DIR)

GTEST_SRCS=$(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJS=$(subst .cc,.o,$(GTEST_SRCS))

GMOCK_SRCS=$(GMOCK_DIR)/src/gmock-all.cc
GMOCK_OBJS=$(subst .cc,.o,$(GMOCK_SRCS))

GTEST_OUT=$(GTEST_DIR)/libgtest.a

DEPENDS=$(GTEST_OUT)

EXT_INC=$(GTEST_INC) $(GMOCK_INC)
SRC_INC=src
INC=$(SRC_INC) $(EXT_INC)
CPPFLAGS=$(foreach d, $(INC), -I$d) $(GTEST_NO_PTHREAD)
TCPPFLAGS=$(CPPFLAGS) -coverage -D_TEST
GIT_FLAGS=-c advice.detachedHead=false --depth=1

TEST_DIR=tests
TEST_SRCS=$(shell find $(TEST_DIR) -name *.cpp)
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS))
TEST_OUT=$(TEST_DIR)/tests.exe
TEST_LD_FLAGS=-L $(GTEST_DIR) -lgtest -coverage

SRC_DIR=src
SRCS=$(shell find $(SRC_DIR) -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))
TOBJS=$(subst .o,.t.o,$(OBJS))

AR=ar
GIT=git
CPP=g++
SHELL=/bin/bash

.PHONY: codecov default depend gcov test test-tidy test-clean clean dist-clean

default: all

$(GTEST_BASE_DIR):
	$(GIT) clone $(GIT_FLAGS) $(GTEST_REPO) -b $(GTEST_TAG) $@

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

%.t.o: %.cpp $(DEPENDS)
	$(CPP) $(TCPPFLAGS) -c $< -o $@

$(TEST_OUT): $(TEST_OBJS) $(TOBJS)
	$(CPP) $(TEST_OBJS) $(TOBJS) -o $@ $(TEST_LD_FLAGS)

test: $(TEST_OUT)
	GTEST_OUTPUT=xml:test-results.xml $<

gcov: test
	gcov -r $(TOBJS)

codecov: gcov
	curl -Os https://uploader.codecov.io/latest/linux/codecov
	chmod +x codecov
	./codecov -X gcov

test-tidy:
	rm -f $(TEST_OBJS)

test-clean: test-tidy
	rm -f $(TEST_OUT)
	rm -f $(TOBJS)
	rm -f $(SRC_DIR)/*.{gcda,gcov,gcno}
	rm -f $(TEST_DIR)/*.{gcda,gcov,gcno}

tidy: test-tidy
	rm -f $(OBJS)

clean: tidy test-clean

dist-clean: clean
	rm -rf $(EXT_DIR)
