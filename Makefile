# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I"./include" -I"./include/sdl3"
LDFLAGS = -L"./include/sdl3" -lSDL3

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
SIMULATOR_DIR = $(SRC_DIR)\simulator
GENERATOR_DIR = $(SRC_DIR)\generator

# Create lists of source files
SIMULATOR_SRCS = $(SIMULATOR_DIR)\simulator.c \
                 $(SIMULATOR_DIR)\queue.c \
                 $(SIMULATOR_DIR)\visualization.c

GENERATOR_SRCS = $(GENERATOR_DIR)\traffic_generator.c \
                 $(SIMULATOR_DIR)\queue.c

# Executable names
SIMULATOR = $(BUILD_DIR)\simulator.exe
GENERATOR = $(BUILD_DIR)\generator.exe

# Default target
all: dirs $(SIMULATOR) $(GENERATOR)

# Create build directories
dirs:
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	if not exist "$(BUILD_DIR)\simulator" mkdir "$(BUILD_DIR)\simulator"
	if not exist "$(BUILD_DIR)\generator" mkdir "$(BUILD_DIR)\generator"

# Build rules for executables
$(SIMULATOR): $(SIMULATOR_SRCS)
	$(CC) $(SIMULATOR_SRCS) -o $@ $(CFLAGS) $(LDFLAGS)

$(GENERATOR): $(GENERATOR_SRCS)
	$(CC) $(GENERATOR_SRCS) -o $@ $(CFLAGS)

# Clean build files
clean:
	if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"

# Run the simulation
run: $(SIMULATOR) $(GENERATOR)
	start "" "$(GENERATOR)"
	"$(SIMULATOR)"

.PHONY: all clean run dirs