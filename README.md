# .gitignore

_.o
_.exe
_.out
.vscode/
.DS_Store
build/
data/_.txt

# README.md

# Traffic Junction Queue Simulator

A traffic junction simulation implementing queue data structures to manage traffic flow at a four-way intersection.

## Features

- Queue-based traffic management system
- Priority lane handling
- Real-time visualization using SDL2
- Traffic generation system

## Building the Project

```bash
make
```

## Running the Simulation

1. Start the traffic generator:

```bash
./traffic_generator
```

2. In another terminal, start the simulator:

```bash
./simulator
```

## Project Structure

- `src/simulator/` - Main simulation code
- `src/generator/` - Traffic generation system
- `data/` - Communication files between generator and simulator
- `include/` - External dependencies

## Requirements

- SDL2 library
- C compiler (gcc recommended)
