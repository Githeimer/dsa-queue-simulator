#include "../include/priority_queue.h"

int main()
{
    PriorityQueue *trafficQueue = createPriorityQueue();
    enqueue(trafficQueue, 1, 1, NORMAL_PRIORITY);
    enqueue(trafficQueue, 2, 2, NORMAL_PRIORITY);

    enqueue(trafficQueue, 3, 3, HIGH_PRIORITY);
    enqueue(trafficQueue, 4, 4, HIGH_PRIORITY);

    printf("Current Queue:\n");
    displayQueue(trafficQueue);

    Vehicle *v = dequeue(trafficQueue);
    if (v)
    {
        printf("\nDequeued Vehicle ID: %d (Priority: %d)\n", v->id, v->priority);
        free(v);
    }

    printf("\nQueue after one dequeue:\n");
    displayQueue(trafficQueue);

    freeQueue(trafficQueue);
    return 0;
}
