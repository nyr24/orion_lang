SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = include
OBJS_NAMES = main.o orion_memory.o debug.o chunk.o value.o vm.o scanner.o compiler.o
OBJS_WITH_PATH = $(BUILD_DIR)/main.o $(BUILD_DIR)/orion_memory.o $(BUILD_DIR)/debug.o \
	   $(BUILD_DIR)/chunk.o $(BUILD_DIR)/value.o $(BUILD_DIR)/vm.o \
	   $(BUILD_DIR)/scanner.o $(BUILD_DIR)/compiler.o
CFLAGS = -I$(INCLUDE_DIR) -I$(BUILD_DIR) -Wall -std=c17 --debug
CC = clang
EXEC_NAME = app
VPATH = $(SRC_DIR) $(INCLUDE_DIR) $(BUILD_DIR)


all: $(OBJS_NAMES)
	$(CC) -o $(BUILD_DIR)/$(EXEC_NAME) $(CFLAGS) $(OBJS_WITH_PATH)
	echo "Build is ready!"

main.o: main.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main.o -c $(SRC_DIR)/main.c

orion_memory.o: orion_memory.c orion_memory.h common.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/orion_memory.o -c $(SRC_DIR)/orion_memory.c

debug.o: debug.c debug.h common.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/debug.o -c $(SRC_DIR)/debug.c

chunk.o: chunk.c chunk.h common.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/chunk.o -c $(SRC_DIR)/chunk.c

value.o: value.c value.h common.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/value.o -c $(SRC_DIR)/value.c

vm.o: vm.c vm.h common.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/vm.o -c $(SRC_DIR)/vm.c

scanner.o: scanner.c scanner.h vm.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/scanner.o -c $(SRC_DIR)/scanner.c

compiler.o: compiler.c compiler.h scanner.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/compiler.o -c $(SRC_DIR)/compiler.c

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(EXEC_NAME) $(BUILD_DIR)/*.exe
	echo "Build dir cleaned up!"
