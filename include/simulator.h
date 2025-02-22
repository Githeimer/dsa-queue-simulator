#ifndef SIMULATION_H
#define SIMULATION_H

#include "lane.h"

void initSimulation();
void runSimulation();
void updateVehiclePositions(Lane *lane);
void processTrafficLights(Lane *lane);
void simulateTrafficMovement(Lane *lane);

#endif // SIMULATION_H
