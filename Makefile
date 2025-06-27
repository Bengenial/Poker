CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCS = src/poker.c src/juegoPoker/*.c src/tdas/*.c
TARGET_DIR = build
TARGET = $(TARGET_DIR)/poker

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all run clean