#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>                                                             
#include <unistd.h>
#include <time.h>
#include <stdlib.h>



/*Escriba un programa monohilo que calcule el producto entre 4 pares de vectores de longitud N, donde N es un dato de entrada y N >= 1000.*/

//declaracion de los 4 pares de vectores globales
int *vector1;
int *vector2;
int *vector3;
int *vector4;
int *vector5;
int *vector6;
int *vector7;
int *vector8;

void *proceso1(void *n){ 
    int tama = *((int *)n);
    int i;
    long long int resultado1 = 0;
    long long int resultado2 = 0;
    long long int resultado3 = 0;
    long long int resultado4 = 0;
    //asignamos memoria a los vectores
    vector1 = (int *)malloc(tama* sizeof(int));
    vector2 = (int *)malloc(tama* sizeof(int));
    vector3 = (int *)malloc(tama* sizeof(int));
    vector4 = (int *)malloc(tama* sizeof(int));
    vector5 = (int *)malloc(tama* sizeof(int));
    vector6 = (int *)malloc(tama* sizeof(int));
    vector7 = (int *)malloc(tama* sizeof(int));
    vector8 = (int *)malloc(tama* sizeof(int));

    for(i=0 ; i < tama ; i++){
        vector1[i] = rand() % 10000; 
    }
    for(i =0 ; i < tama ; i++){
        vector2[i] = rand() % 10000;
    }
     for(i=0 ; i < tama ; i++){
        vector8[i] = rand() % 10000; 
    }
    for(i =0 ; i < tama ; i++){
        vector3[i] = rand() % 10000;
    }
    for(i=0 ; i < tama ; i++){
        vector4[i] = rand() % 10000; 
    }
    for(i =0 ; i < tama ; i++){
        vector5[i] = rand() % 10000;
    }
     for(i=0 ; i < tama ; i++){
        vector6[i] = rand() % 10000; 
    }
    for(i =0 ; i < tama ; i++){
        vector7[i] = rand() % 10000;
    }
    //realiza la suma
    for(i = 0 ; i < tama ; i++){
        resultado1 += vector1[i] * vector2[i];
    }
    for(i = 0 ; i < tama ; i++){
        resultado2 += vector3[i] * vector4[i];
    }
    for(i = 0 ; i < tama ; i++){
        resultado3 += vector5[i] * vector6[i];
    }
    for(i = 0 ; i < tama ; i++){
        resultado4 += vector7[i] * vector8[i];
    }
    printf("El resultado del producto punto entre el vector 1 y vector 2 es: %lld\n",resultado1);
    printf("El resultado del producto punto entre el vector 3 y vector 4 es: %lld\n",resultado2);
    printf("El resultado del producto punto entre el vector 5 y vector 6 es: %lld\n",resultado3);
    printf("El resultado del producto punto entre el vector 7 y vector 8 es: %lld\n",resultado4);
    pthread_exit(NULL);
}


int main()
{
    clock_t inicio, fin;
    double tiempo_transcurrido;
    int n;

    printf("Ingresa el tamaño de los vectores(debe ser mayor o igual a 1000):");
    scanf("%d",&n);
    srand(time(NULL));
    if(n>=1000){
        inicio = clock(); //mide el inicio del tiempo de ejecucion

        pthread_t hilo1;
        pthread_create(&hilo1, NULL, proceso1, (void *)&n);
        pthread_join(hilo1, NULL);

        fin = clock();  //mide el fin de tiempo de ejecucion
    }else
        printf("El valor ingresado debe ser mayor o igual a 1000\n");
        
    tiempo_transcurrido = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    printf("El tiempo de ejecución fue de %.6f segundos\n", tiempo_transcurrido);
    return 0;
}

