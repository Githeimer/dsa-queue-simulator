CC = gcc
CFLAGS = -Wall -Wextra -Iinclude  

SRC = src/main.c src/priority_queue.c
OBJ = $(SRC:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)
