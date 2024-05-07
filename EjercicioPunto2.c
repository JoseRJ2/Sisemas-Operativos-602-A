#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>                                                             
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

//programa el cual manda a imprimir diferentes archivos a una "impresora" compartida, utilizando candados, para que no tenga que crear los archivos, yo los creo y uso dentro del codigo.
//Jose Guadalupe Robles Jimenez, 602-A

pthread_mutex_t candado;

void *impresora(void *n){
    char *nombreArchivo=(char *)n;
    pthread_mutex_lock(&candado);      //este candado hace que se impriman los archivos de uno por uno
    FILE *archivo = fopen(nombreArchivo,"r");
    char imprime[1000];
    if(archivo == NULL){
        printf("Error al abrir el archivo");
        exit(0);
    }
    printf("\nContenido del archivo de nombre  %s:  \n",nombreArchivo);
    while(fgets(imprime,sizeof(imprime),archivo)!=NULL){
        printf("%s\n",imprime);
    }
    pthread_mutex_unlock(&candado);    //cierra el candado
    pthread_exit(NULL);
}

int main() {
    FILE *archivo;
    pthread_t hilo1,hilo2,hilo3,hilo4;
    pthread_mutex_init(&candado,NULL);
    char cadena1[] = "Hola mundo este es el archivo 1";
    char cadena2[] = "Hola mundo este es el archivo 2";
    char cadena3[] = "Hola mundo este es el archivo 3";
    char cadena4[] = "Hola mundo este es el archivo 4";
    char *archivos[]= {"archivo1.txt","archivo2.txt","archivo3.txt","archivo4.txt"};

    //creamos el archivo 1 que se imprimira en el hilo 1
    archivo = fopen("archivo1.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1; 
    }
    fprintf(archivo,"%s", cadena1);
    fclose(archivo);

    //creamos el archivo 2 que se imprimira en el hilo 2
    archivo = fopen("archivo2.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1; 
    }
    fprintf(archivo,"%s", cadena2);
    fclose(archivo);

    //creamos el archivo 3 que se imprimira en el hilo 3
    archivo = fopen("archivo3.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1; 
    }
    fprintf(archivo,"%s", cadena3);
    fclose(archivo);

    //creamos el archivo 4 que se imprimira en el hilo 4
    archivo = fopen("archivo4.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1; 
    }
    fprintf(archivo,"%s", cadena4);
    fclose(archivo);

    //mandamos el nombre del archivo a la impresora 
    pthread_create(&hilo1,NULL,impresora,(void *)archivos[0]);
    pthread_create(&hilo2,NULL,impresora,(void *)archivos[1]);
    pthread_create(&hilo3,NULL,impresora,(void *)archivos[2]);
    pthread_create(&hilo4,NULL,impresora,(void *)archivos[3]);
    pthread_join(hilo1,NULL);
    pthread_join(hilo2,NULL);
    pthread_join(hilo3,NULL);
    pthread_join(hilo4,NULL);


    return 0;
}
