#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_FILOSOFOS 5
#define NUM_TENEDORES 5
#define MAX_CAMAREROS 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicion = PTHREAD_COND_INITIALIZER;

int tenedores[NUM_TENEDORES];
int camareros_disponibles = MAX_CAMAREROS;

void *filosofo(void *arg) {
    int id = *((int *) arg);
    int tenedor_izquierdo = id;
    int tenedor_derecho = (id + 1) % NUM_TENEDORES;
    
    while(1) {
        // Solicita al camarero
        pthread_mutex_lock(&mutex);
        while (camareros_disponibles == 0) {
            pthread_cond_wait(&condicion, &mutex);
        }
        camareros_disponibles--;
        pthread_mutex_unlock(&mutex);
        
        // Toma los tenedores
        pthread_mutex_lock(&mutex);
        tenedores[tenedor_izquierdo] = 1;
        tenedores[tenedor_derecho] = 1;
        pthread_mutex_unlock(&mutex);
        
        // Come
        printf("Filósofo %d comiendo\n", id);
        sleep(rand() % 3); // Simula tiempo de comer
        
        // Libera los tenedores
        pthread_mutex_lock(&mutex);
        tenedores[tenedor_izquierdo] = 0;
        tenedores[tenedor_derecho] = 0;
        
        // Libera al camarero
        camareros_disponibles++;
        pthread_cond_signal(&condicion);
        pthread_mutex_unlock(&mutex);
        
        // Piensa
        printf("Filósofo %d pensando\n", id);
        sleep(rand() % 3); // Simula tiempo de pensar
    }
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];
    int i;

    // Inicializa los tenedores
    for (i = 0; i < NUM_TENEDORES; i++) {
        tenedores[i] = 0;
    }

    // Crea los hilos de los filósofos
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    // Espera a que los filósofos terminen
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    return 0;
}
