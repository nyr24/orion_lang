SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = include
OBJS = main.o orion_memory.o debug.o chunk.o value.o vm.o
CFLAGS = -I$(INCLUDE_DIR) -Wall -std=c17 --debug
CC = clang
VPATH = $(SRC_DIR) $(INCLUDE_DIR)

all_dbg: $(OBJS)
	$(CC) -o $(BUILD_DIR)/app $(CFLAGS) $(OBJS)
	echo "Build is ready!"

main.o: main.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c

orion_memory.o: orion_memory.c orion_memory.h common.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/orion_memory.c

debug.o: debug.c debug.h common.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/debug.c

chunk.o: chunk.c chunk.h common.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/chunk.c

value.o: value.c value.h common.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/value.c

vm.o: vm.c vm.h common.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/vm.c
