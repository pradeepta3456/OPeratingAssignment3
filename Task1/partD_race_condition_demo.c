#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

long unsafe_counter = 0; /* NOT protected by a mutex - this is the bug */

void *unsafe_worker(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 5000000; i++) {
        unsafe_counter++; /* read-modify-write with no protection */
    }
    printf("[Thread %d] finished (unsafe).\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS] = {1, 2, 3, 4};

    printf("=== Part D: Demonstrating an UNPROTECTED race condition ===\n");
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, unsafe_worker, &ids[i]);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Final unsafe_counter = %ld (expected %d)\n",
       unsafe_counter, NUM_THREADS * 5000000);
    printf("If this number is LESS than expected, updates were lost - that is the race condition.\n");

    return 0;
}
