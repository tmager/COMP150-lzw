BIN_DIR=../bin
OBJ_DIR=../obj

CC = g++
LANG = -std=c++11
CFLAGS_ADDL = 
CFLAGS = $(LANG) $(CFLAGS_ADDL) -Wall -Wextra -Werror -O2 -g3

SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst tests/%.cpp, $(OBJ_DIR)/%, $(SOURCES))

all: $(BIN_DIR)/lzw

$(BIN_DIR)/lzw: $(OBJ_DIR)/lzw.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/trienode.o \
			$(OBJ_DIR)/dictionaryentry.o $(OBJ_DIR)/bitseq.o \
			$(OBJ_DIR)/compressor.o $(OBJ_DIR)/dictionary.o \
			$(OBJ_DIR)/dictionary_static.o \
			$(OBJ_DIR)/compressor_fixed.o \
			$(OBJ_DIR)/compressor_fixed_static.o \
			$(OBJ_DIR)/compressor_variable.o \
			$(OBJ_DIR)/compressor_variable_static.o \
			| $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@

clean::
	rm -rf $(OBJ_DIR) $(BIN_DIR)
