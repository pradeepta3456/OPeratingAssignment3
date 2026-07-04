#include <stdio.h>

typedef struct {
    int pid;
    int burst_time;
    int remaining;
} Process;

void round_robin_simulation(void) {
    int quantum = 2;
    Process procs[3] = {
        {1, 5, 5},
        {2, 3, 3},
        {3, 7, 7}
    };
    int n = 3;
    int time = 0;
    int done;

    printf("--- Round-Robin Scheduler Simulation (quantum = %d) ---\n", quantum);
    do {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (procs[i].remaining > 0) {
                done = 0;
                int slice = (procs[i].remaining < quantum) ? procs[i].remaining : quantum;
                printf("Time %2d-%2d : Process P%d runs (remaining before=%d)\n",
                       time, time + slice, procs[i].pid, procs[i].remaining);
                time += slice;
                procs[i].remaining -= slice;
                if (procs[i].remaining == 0)
                    printf("           -> P%d completed at time %d\n", procs[i].pid, time);
            }
        }
    } while (!done);
    printf("--- Simulation complete. Total time = %d ---\n", time);
}

int main(void) {
    round_robin_simulation();
    return 0;
}
