SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = include
OBJS = main.o orion_memory.o debug.o chunk.o value.o
CFLAGS = -I$(INCLUDE_DIR) -Wall -std=c17 --debug
CC = clang
VPATH = $(SRC_DIR) $(INCLUDE_DIR)

all_dbg: $(OBJS)
	$(CC) -o $(BUILD_DIR)/app $(CFLAGS) $(OBJS)
	echo "Build is ready!"

main.o: main.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c

orion_memory.o: orion_memory.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/orion_memory.c

debug.o: debug.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/debug.c

chunk.o: chunk.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/chunk.c

value.o: value.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/value.c
