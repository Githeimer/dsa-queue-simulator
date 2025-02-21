CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -lSDL3
EXEC = main
SRC = main.c src/simulator/traffic_simulator.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(EXEC)
