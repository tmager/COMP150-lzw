CC = g++
LANG = -std=c++11
CFLAGS_ADDL = 
CFLAGS = $(LANG) $(CFLAGS_ADDL) -Wall -Wextra -Werror -O3 -g3
TESTS = $(wildcard UT_*.cpp)
TEST_OBJS = $(patsubst %.cpp, %, $(TESTS))

all: tests lzw

tests: $(TEST_OBJS)

lzw: lzw.cpp utils.cpp trienode.cpp dictionaryentry.cpp bitseq.cpp \
			compressor.cpp dictionary.cpp \
			compressor_fixed.cpp compressor_fixed_static.cpp \
			compressor_variable.cpp compressor_variable_static.cpp \
			dictionary_static.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean::
	rm -f lzw $(TEST_OBJS)
