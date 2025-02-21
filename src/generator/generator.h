// traffic_generator/include/Generator.h
#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "core/Constants.h"

typedef struct
{
    double spawnProbability;
    int maxVehicles;
    char name[100];
} LaneSettings;

typedef struct
{
    uint32_t nextVehicleId;
    char dataDir[256];
    time_t lastGenTime;
    LaneSettings laneSettings[MAX_LANES];
} Generator;

// Generation function ko prototypes haru
Direction generateRandomDirection();
void writeVehicleToFile(const char *filepath, uint32_t id, Direction dir);
size_t countVehiclesInFile(const char *filepath);
void initializeLaneSettings(Generator *gen);
bool shouldGenerateVehicle(Generator *gen, int laneId, size_t currentCount);
void clearAllFiles(Generator *gen);
void generateTraffic(Generator *gen);
void displayStatus(const Generator *gen);

#endif // GENERATOR_H
