#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


/*Escribir un programa multihilo que calcule el producto entre 4 pares de vectores de longitud N, donde N es un dato de entrada y N >= 1000.*/


#define VECTOR_SIZE 4
#define NUM_HILOS 4

// Estructura para pasar los argumentos a los hilos
struct Args {
    int *vector1;
    int *vector2;
    long long int resultado;
    int tama;
};

// Función para calcular el producto punto entre dos vectores
void *producto_punto(void *arg) {
    struct Args *args = (struct Args *)arg;
    args->resultado = 0;
    for (int i = 0; i < args->tama; ++i) {
        args->resultado += args->vector1[i] * args->vector2[i];
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_HILOS];
    struct Args args[NUM_HILOS];
    clock_t inicio, fin;
    double tiempo_transcurrido;
    int i,n;
    printf("Dame el tamaño de los vectores(tien que se mayor o igual a 1000):");
    scanf("%d",&n);
    if(n >= 1000){
        // Definir los 4 pares de vectores
        int *vector1 = (int *)malloc(n* sizeof(int));
        int *vector2 = (int *)malloc(n* sizeof(int));
        int *vector3 = (int *)malloc(n* sizeof(int));
        int *vector4 = (int *)malloc(n* sizeof(int));

        srand(time(NULL));
        for(i=0 ; i < n; i++){
            vector1[i] = rand() % 10000; 
        }
        for(i =0 ; i < n ; i++){
            vector2[i] = rand() % 10000;
        }
        for(i =0 ; i < n; i++){
            vector3[i] = rand() % 10000;
        }
        for(i=0 ; i < n; i++){
            vector4[i] = rand() % 10000; 
        }
        // Crear los hilos y pasar los argumentos para calcular el producto punto
        inicio = clock(); //mide el inicio del tiempo de ejecucion
        args[0].vector1 = vector1;
        args[0].vector2 = vector2;
        args[0].tama = n;
        pthread_create(&hilos[0], NULL, producto_punto, (void *)&args[0]);

        args[1].vector1 = vector3;
        args[1].vector2 = vector4;
        args[1].tama = n;
        pthread_create(&hilos[1], NULL, producto_punto, (void *)&args[1]);

        args[2].vector1 = vector2;
        args[2].vector2 = vector3;
        args[2].tama = n;
        pthread_create(&hilos[2], NULL, producto_punto, (void *)&args[2]);

        args[3].vector1 = vector4;
        args[3].vector2 = vector1;
        args[3].tama = n;
        pthread_create(&hilos[3], NULL, producto_punto, (void *)&args[3]);

        // Esperar a que todos los hilos terminen
        for (int i = 0; i < NUM_HILOS; ++i) {
            pthread_join(hilos[i], NULL);
        }
        fin = clock();  //mide el fin de tiempo de ejecucion
        // Imprimir los resultados
        printf("Producto punto entre vector1 y vector2: %lld\n", args[0].resultado);
        printf("Producto punto entre vector3 y vector4: %lld\n", args[1].resultado);
        printf("Producto punto entre vector2 y vector3: %lld\n", args[2].resultado);
        printf("Producto punto entre vector4 y vector1: %lld\n", args[3].resultado);
        tiempo_transcurrido = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
        printf("El tiempo de ejecución fue de %.6f segundos\n", tiempo_transcurrido);
        free(vector1);
        free(vector2);
        free(vector3);
        free(vector4);
    }else{
        printf("El tamaño no es mayor o igual a 1000");
    }
    return 0;
}
