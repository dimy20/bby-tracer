EXEC = main
CC = gcc

SRC_DIR = ./src
BUILD_DIR = ./build
INCLUDE_DIRS = ./include

CFLAGS = -Werror -Wall -Wconversion -g -std=c11 $(foreach D, $(INCLUDE_DIRS), -I$(D))
LDFLAGS = -lm

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

$(EXEC): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm $(BUILD_DIR)/%.o $(EXEC)

