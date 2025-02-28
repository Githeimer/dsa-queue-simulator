CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -lSDL3
EXEC_GENERATOR = generator
EXEC_SIMULATOR = simulator

SRC_GENERATOR = src/generator/generator.c src/generator/run_generator.c src/vehicle/vehicle.c src/lane/lane.c
SRC_SIMULATOR = src/simulator/simulation.c src/simulator/vehicleQueue.c src/lane/laneSimulation.c src/simulator/traffic.c

all: $(EXEC_GENERATOR) $(EXEC_SIMULATOR)

# Build the generator
$(EXEC_GENERATOR): $(SRC_GENERATOR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build the simulator
$(EXEC_SIMULATOR): $(SRC_SIMULATOR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(EXEC_GENERATOR) $(EXEC_SIMULATOR)
