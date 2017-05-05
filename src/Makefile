CC = g++
LANG = -std=c++11
CFLAGS_ADDL = 
CFLAGS = $(LANG) $(CFLAGS_ADDL) -Wall -Wextra -Werror -g3 -O0
TESTS = $(wildcard UT_*.cpp)
TEST_OBJS = $(patsubst %.cpp, %, $(TESTS))

all: tests lzw

tests: $(TEST_OBJS)

lzw: lzw.cpp utils.cpp dictionary.cpp dictionaryentry.cpp bitseq.cpp \
			compressor.cpp compressor_fixed_static.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean::
	rm -f lzw $(TEST_OBJS)
