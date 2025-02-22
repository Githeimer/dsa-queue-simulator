CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -lSDL3
EXEC = generator

SRC = src/generator/generator.c src/generator/run_generator.c src/vehicle/vehicle.c src/lane/lane.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(EXEC)
