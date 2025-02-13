#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_VEHICLES 100

void generateTraffic()
{
    FILE *file = fopen("vehicles.data", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    srand(time(NULL));

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        int lane = rand() % 3;
        int priority = (rand() % 10 > 7) ? 1 : 0;
        fprintf(file, "Vehicle ID: %d, Lane: %d, Priority: %d\n", i, lane, priority);
    }

    fclose(file);
}

int main()
{
    generateTraffic();
    printf("Traffic data generated in 'vehicles.data'.\n");
    return 0;
}
