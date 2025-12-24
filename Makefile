CC = gcc
CFLAGS = -Wall -O2 -pthread -Iinclude
SRCS = main.c core/socks5.c core/server.c
OBJS = $(patsubst %.c,build/%.o,$(SRCS))
TARGET = out/chat

# Create build/ subdirectories
$(shell mkdir -p build/core build out)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/* out/*

.PHONY: all clean
