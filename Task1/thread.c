#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

long shared_counter = 0;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&counter_lock);
        shared_counter++;
        pthread_mutex_unlock(&counter_lock);
    }
    printf("[Thread %d] finished 100000 increments.\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS] = {1, 2, 3, 4};

    printf("=== Part A: Threads + Mutex (race condition prevention) ===\n");
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Final shared_counter = %ld (expected %d)\n",
           shared_counter, NUM_THREADS * 100000);

    return 0;
}
