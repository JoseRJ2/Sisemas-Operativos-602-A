#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

/*En clase se explicó y se programó una solución al problema de los cinco filósofos comensales. Programe y explique dos soluciones alternativas a ese problema.
En esta segunda solucion abordo la solucion de la asimetria que basicamente consiste en lugar de intentar tomar ambos tenedores al mismo tiempo, un filósofo primero intenta tomar el tenedor a su izquierda y luego el tenedor a su derecha (o viceversa).*/

#define NUM_FILOSOFOS 5
#define TIEMPO_COMER_MAXIMO 3

pthread_mutex_t tenedores[NUM_FILOSOFOS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *filosofo(void *arg) {
    int id = *((int *) arg);
    int tenedor_izquierdo = id;
    int tenedor_derecho = (id + 1) % NUM_FILOSOFOS;
    
    while(1) {
        // Filósofos impares toman primero el tenedor derecho, luego el izquierdo
        if (id % 2 == 0) {
            pthread_mutex_lock(&tenedores[tenedor_derecho]);
            pthread_mutex_lock(&tenedores[tenedor_izquierdo]);
        } else { // Filósofos pares toman primero el tenedor izquierdo, luego el derecho
            pthread_mutex_lock(&tenedores[tenedor_izquierdo]);
            pthread_mutex_lock(&tenedores[tenedor_derecho]);
        }
        
        // Come
        printf("Filósofo %d comiendo\n", id);
        sleep(rand() % (TIEMPO_COMER_MAXIMO + 1)); // Simula tiempo de comer
        
        // Libera los tenedores
        pthread_mutex_unlock(&tenedores[tenedor_izquierdo]);
        pthread_mutex_unlock(&tenedores[tenedor_derecho]);
        
        // Piensa
        printf("Filósofo %d pensando\n", id);
        sleep(rand() % (TIEMPO_COMER_MAXIMO + 1)); // Simula tiempo de pensar
    }
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];
    int i;

    // Inicializa los mutex para los tenedores
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedores[i], NULL);
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

    // Destruye los mutex para los tenedores
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    return 0;
}
