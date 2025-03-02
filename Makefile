# General
SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = include
CC = clang
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -std=c17
VPATH = $(SRC_DIR) $(INCLUDE_DIR) $(BUILD_DIR)
SRCS = main.c orion_memory.c debug.c chunk.c value.c vm.c scanner.c compiler.c object.c
OBJS = $(SRCS:.c=.o)
EXE = app

# Debug
DBG_DIR = $(BUILD_DIR)/debug
DBG_EXE = $(DBG_DIR)/$(EXE)
DBG_OBJS = $(addprefix $(DBG_DIR)/,$(OBJS))
DBG_FLAGS = $(CFLAGS) -g -O0 -DDEBUG

# Release
REL_DIR = $(BUILD_DIR)/release
REL_EXE = $(REL_DIR)/$(EXE)
REL_OBJS = $(addprefix $(REL_DIR)/,$(OBJS))
REL_FLAGS = $(CFLAGS) -O3 -DNDEBUG

# Targets

.PHONY: all prep debug release clean

all: prep debug

# Debug

debug: $(DBG_EXE)

$(DBG_EXE): $(DBG_OBJS)
	$(CC) $(DBG_FLAGS) -o $@ $^

$(DBG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(DBG_FLAGS) -o $@ $<

# Release

release: prep $(REL_EXE)

$(REL_EXE): $(REL_OBJS)
	$(CC) $(REL_FLAGS) -o $@ $^

$(REL_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(REL_FLAGS) -o $@ $<

# Util
prep:
	mkdir -p $(DBG_DIR) $(REL_DIR)

clean:
	rm -f $(REL_OBJS) $(REL_EXE) $(DBG_OBJS) $(DBG_EXE)
