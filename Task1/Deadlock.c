#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lockX = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockY = PTHREAD_MUTEX_INITIALIZER;

void *safe_transfer_a(void *arg) {
    (void)arg;
    pthread_mutex_lock(&lockX);
    usleep(1000);
    pthread_mutex_lock(&lockY);
    printf("[safe_transfer_a] acquired X then Y safely.\n");
    pthread_mutex_unlock(&lockY);
    pthread_mutex_unlock(&lockX);
    return NULL;
}

void *safe_transfer_b(void *arg) {
    (void)arg;
    pthread_mutex_lock(&lockX);
    usleep(1000);
    pthread_mutex_lock(&lockY);
    printf("[safe_transfer_b] acquired X then Y safely.\n");
    pthread_mutex_unlock(&lockY);
    pthread_mutex_unlock(&lockX);
    return NULL;
}

int main(void) {
    printf("=== Part B: Deadlock prevention via fixed lock ordering ===\n");

    pthread_t tA, tB;
    pthread_create(&tA, NULL, safe_transfer_a, NULL);
    pthread_create(&tB, NULL, safe_transfer_b, NULL);
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    return 0;
}
