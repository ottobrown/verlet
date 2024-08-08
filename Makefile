CC = gcc

CFLAGS = -Wall -Werror -Iinclude

SRCS = src/main.c src/verlet.c src/world.c src/link.c

OBJS = $(SRCS:.c=.o)

TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./a.out

.PHONY: all clean
