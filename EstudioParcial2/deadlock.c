#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t sem1, sem2;

void *thread1(void *arg) {
   while(1){
        printf("Thread 1: Waiting for sem1\n");
        sem_wait(&sem1);
        printf("Thread 1: Acquired sem1\n");
        printf("Thread 1: Waiting for sem2\n");
        sem_wait(&sem2);
        printf("Thread 1: Acquired sem2\n");
        sem_post(&sem2);
        sem_post(&sem1);
        sleep(1);
   }
    pthread_exit(NULL);
}

void *thread2(void *arg) {
    while(1){
        printf("Thread 2: Waiting for sem2\n");
        sem_wait(&sem2);
        printf("Thread 2: Acquired sem2\n");
        printf("Thread 2: Waiting for sem1\n");
        sem_wait(&sem1);
        printf("Thread 2: Acquired sem1\n");
        sem_post(&sem1);
        sem_post(&sem2);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid1, tid2;

    sem_init(&sem1, 0, 1); // Inicializar sem1 en 1
    sem_init(&sem2, 0, 1); // Inicializar sem2 en 1

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
