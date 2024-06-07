#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 2
#define ITERATIONS 1000000

int shared_variable = 0;
sem_t semaforo;;

void *increment(void *arg) {
    sem_wait(&semaforo);
    for (int i = 0; i < ITERATIONS; ++i) {
        shared_variable++;
    }
    sem_post(&semaforo);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    sem_init(&semaforo,0,1);
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("The value of the shared variable is: %d\n", shared_variable);

    return 0;
}
