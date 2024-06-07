#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_PAGINA 4096   //DEFINIMOS EL TAMAÑO DE LA PAGINA 4K
#define MEMORIA_TOTAL 32768 //DEFINIMOS LA MEMORIA TOTAL 32K
#define NUM_MARCOS (MEMORIA_TOTAL / TAM_PAGINA) //DEFINIMOS EL NUMERO DE MARCOS SERIAN 8

//DEFINIMOS LAS ESTRUCTURAS QUE UTILIZAREMOS

typedef struct{
    int pid;    //identificador de proceso
    int num_paginas; //numero de paginas que tendra cada proceso
    int *tabla_paginas; //donde se guardaran las paginas de este proceso
}Proceso;

typedef struct{
    int proceso_id; //aqui se mostrara el numero de proceso en la tabla
    int numero_pagina; //aqui se mostrara el numero de pagina que esta asociado a ese proceso 
}Marco;

void agregarProceso(int pid, int num_paginas);
int hay_memoria_suficiente(int num_paginas);
void asignar_memoria(Proceso *proceso);
int encontrar_marco_libre();
void mostrar();
void quitar_proceso(int pid);
void asignar_memoria_esperando();

Marco marcos[8]; //como son 8 marcos creamos un arreglo para mostrar los 8 procesos con su informacion
Proceso *procesos[100];  //podremos crear hasta 100 procesos
int contador_procesos =0;
Proceso *procesos_esperando[100]; //podremos tener otros 100 procesos esperando a que se desocupe la memoria
int contador_esperando =0; //contador de procesos esperando
Proceso *procesos_completados[100]; //guarda los procesos completados
int contador_completados=0; //cuenta los procesos completados

//funcion para añadir un nuevo proceso
void agregarProceso(int pid, int num_paginas){
    Proceso *nuevo_proceso = (Proceso *)malloc(sizeof(Proceso));
    nuevo_proceso->pid = pid;
    nuevo_proceso->num_paginas = num_paginas;
    nuevo_proceso->tabla_paginas = (int *)malloc(sizeof(int) * num_paginas);

    if(hay_memoria_suficiente(num_paginas)){
        asignar_memoria(nuevo_proceso);
        procesos[contador_procesos] = nuevo_proceso;
        contador_procesos++;
    }else{
        procesos_esperando[contador_esperando] = nuevo_proceso;
        contador_esperando++;
    } 
}

//funcion para verificar si hay memoria suficiente
int hay_memoria_suficiente(int num_paginas){
    int marcos_disponibles = 0;
    for(int i=0; i < NUM_MARCOS; i++){
        if(marcos[i].proceso_id == -1){
            marcos_disponibles++;
        }
    }
    return marcos_disponibles >= num_paginas;
}

void asignar_memoria(Proceso *proceso){
    for(int i=0; i < proceso->num_paginas; i++){
        int marco = encontrar_marco_libre();
        if(marco != -1){
            marcos[marco].proceso_id = proceso->pid;
            marcos[marco].numero_pagina = i;
            proceso->tabla_paginas[i] = marco;
        }
    }
}

int encontrar_marco_libre(){
    for(int i=0; i < NUM_MARCOS; i++){
        if(marcos[i].proceso_id == -1){
            return i;
        }
    }
    return -1;
}

void mostrar(){
    printf("Procesos en ejecucion:\n");
    for(int i=0; i < contador_procesos; i++){
       printf("---PID: %d, Numero de paginas: %d\n", procesos[i]->pid, procesos[i]->num_paginas);
    }

    printf("\nEstado de la memoria:\n");
    for(int i=0; i < NUM_MARCOS; i++){
        printf("Marco %d: ", i);
        if(marcos[i].proceso_id == -1){
            printf("Libre\n");
        }else{
            printf("PID: %d, Pagina: %d\n", marcos[i].proceso_id, marcos[i].numero_pagina);
        }
    }

    printf("\nProcesos esperando:\n");
    for(int i=0; i < contador_esperando; i++){
        printf("---PID: %d, Numero de paginas: %d\n", procesos_esperando[i]->pid, procesos_esperando[i]->num_paginas);
    }

    printf("\nProcesos completados:\n");
    for(int i=0; i < contador_completados; i++){
        printf("---PID: %d, Numero de paginas: %d\n", procesos_completados[i]->pid, procesos_completados[i]->num_paginas);
    }
}

void quitar_proceso(int pid){
    for(int i=0; i < contador_procesos; i++){
        if(procesos[i]->pid == pid){
            for(int j=0; j < procesos[i]->num_paginas; j++){
                int marco = procesos[i]->tabla_paginas[j];
                marcos[marco].proceso_id = -1;
                marcos[marco].numero_pagina = -1;
            }
            procesos_completados[contador_completados] = procesos[i];
            contador_completados++;
            for(int j=i; j < contador_procesos - 1; j++){
                procesos[j] = procesos[j + 1];
            }
            contador_procesos--;
            asignar_memoria_esperando();
            break;
        }
    }

}

void asignar_memoria_esperando(){
    int i = 0;
    while(i < contador_esperando){
        if(hay_memoria_suficiente(procesos_esperando[i]->num_paginas)){
            asignar_memoria(procesos_esperando[i]);
            procesos[contador_procesos] = procesos_esperando[i];
            contador_procesos++;
            for(int j=i; j < contador_esperando - 1; j++){
                procesos_esperando[j] = procesos_esperando[j + 1];
            }
            contador_esperando--;
        }else{
            i++;
        }
    }
}

int main(){

    int pid, num_paginas, opcion;

    //inicializamos los valores de los marcos
    for(int i=0; i < 8; i++){
        marcos[i].proceso_id = -1;
        marcos[i].numero_pagina = -1;
    }

    while(1){
        printf("\n1. Crear proceso\n");
        printf("2. Terminar proceso\n");
        printf("3. Mostrar estado de memoria\n");
        printf("4. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if(opcion == 1){
            printf("PID: ");
            scanf("%d", &pid);
            printf("Numero de paginas: ");
            scanf("%d", &num_paginas);
            if(num_paginas > 8){
                printf("El numero de paginas no puede ser mayor a 8\n");
                continue;
            }else{
                agregarProceso(pid, num_paginas);
            
            }
            mostrar();
        }else if(opcion == 2){
            printf("PID: ");
            scanf("%d", &pid);
            quitar_proceso(pid);
            mostrar();
        }else if(opcion == 3){
            mostrar();
        }else{
            exit(0); 
        }
    }
    
    return 0;
}