#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *oficina(void *);
int elegirJefe();

#define HILOS 8
pthread_mutex_t lock; // Candado


int numHilo = 1; //Variable global para identificar hilos
int x = 0;//Variable global para ciclo infinito
int jefe;
int llegoElJefe = 0;

int main(){
    int k;
    pthread_t hilo[ HILOS ];
    
    jefe = elegirJefe();
    printf("Id del Jefe : %d (Aleatorio)\n\n", jefe);

    pthread_mutex_init(&lock, NULL);

    for( k = 0; k < HILOS; k++){
        //TAMBIEN PUEDE SER CON UN ARREGLO INDEPENDIENTE DEL CICLO
        int *numHilo_copia = malloc(sizeof(int)); // Crear una copia de numHilo para cada hilo
        *numHilo_copia = numHilo; // Asignar el valor actual de numHilo a la copia
        //printf("%d\n", *numHilo_copia); // Imprimir el valor de la copia
        pthread_create( &hilo[ k ], NULL, oficina, (void *)numHilo_copia); // Pasar la copia al hilo
        numHilo++;
    }

    for( k = 0; k < HILOS; k++){
        pthread_join( hilo[ k ], NULL);
    }

    printf("Fin hilo principal\n");
    return 0;
}

void *oficina(void *s){
    int *hilo = (int *)s;
    
    pthread_mutex_lock(&lock);
    if(*hilo == jefe){
        llegoElJefe = 1;
        printf("%d      Buenos días, espero que tengamos un día lleno de éxito\n", *hilo);
    }else
        if(llegoElJefe == 1)
            printf("%d      Buenos días jefe, los colectivos pasaban llenos\n", *hilo);
        else
            printf("%d      Buenos días compañeros\n", *hilo);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int elegirJefe(){
    srand(time(NULL));
    return rand() % HILOS+1;//Genera un numero aleatorio entre 1 y HILOS
}