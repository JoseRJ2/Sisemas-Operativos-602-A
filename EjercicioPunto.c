#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//Jose Guadalupe Robles Jimenez 602-A

typedef struct {
    unsigned int clave; 
    char nombre[21],
         telefono[11];
    double sueldo;
} Empleado;

typedef struct {
    int in;                 //numero por el cual se va a multiplicar el tamaño del renglon
    int desplazamiento;     //Cantidad de bites por desplazamiento
} Indice;

void CrearArchivoIndice (){
    Indice indice[7];
    int fd = open("index", O_WRONLY|O_CREAT|O_TRUNC, 0600 );
    for(int i=0 ; i < 7 ; i++){
        indice[i].in = i;
        indice[i].desplazamiento = i*sizeof(Empleado);
    }
    write( fd, (void *)indice, 7*sizeof( Indice) );
    close(fd);
}

void intercambiar(Indice *a, Indice *b) {
    Indice temp = *a;
    *a = *b;
    *b = temp;
}

void Ordenar() {
    int fd_index = open("index", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    int fd_personas = open("db.personas", O_RDONLY);
    if (fd_index == -1 || fd_personas == -1) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    Indice indices[7]; // Almacena los índices
    Empleado emp,emp_min;

    // Leer los datos del archivo db.personas y construir los índices
    for (int i = 0; i < 7; i++) {
        Indice indice;
        indice.in = i;
        indice.desplazamiento = i * sizeof(Empleado);
        indices[i] = indice;
    }

    // Ordenar los índices por nombre usando seleccion
    for (int i = 0; i < 6; i++) {
        int min_idx = i;
        for (int j = i + 1; j < 7; j++) {
            lseek(fd_personas, indices[j].desplazamiento, SEEK_SET);
            read(fd_personas, &emp, sizeof(Empleado));

            lseek(fd_personas, indices[min_idx].desplazamiento, SEEK_SET);
            read(fd_personas, &emp_min, sizeof(Empleado));

            if (strcmp(emp.nombre, emp_min.nombre) < 0) {
                min_idx = j;
            }
        }
        intercambiar(&indices[i], &indices[min_idx]);
    }

    // Escribir los índices ordenados en el archivo index
    write(fd_index, &indices, 7*sizeof(Indice));

    // Cerrar los archivos
    close(fd_index);
    close(fd_personas);
}

void ImprimirTabla() {
    int fd_index = open("index", O_RDONLY);
    int fd_personas = open("db.personas", O_RDONLY);
    Indice indice;
    Empleado emp;
    int i=0;
    //solo vamos imprimiendo los datos en orden de empleados
    printf("Tabla  No ordenada por nombre:\n");
    while (read(fd_personas, &emp, sizeof(Empleado)) > 0) {
        lseek(fd_personas, i*sizeof(Empleado), SEEK_SET);
        read(fd_personas, &emp, sizeof(Empleado));
        printf("Clave: %d     , Nombre: %s    , Teléfono: %s    , Sueldo: %.1f    \n", emp.clave,emp.nombre, emp.telefono, emp.sueldo);
        i++;
    }
    //usamos el archivo indice que hicimos para imprimir
    printf("\nTabla ordenada por nombre:\n");
    while (read(fd_index, &indice, sizeof(Indice)) > 0) {
        lseek(fd_personas, indice.desplazamiento, SEEK_SET);
        read(fd_personas, &emp, sizeof(Empleado));
        printf("Clave: %d     , Nombre: %s    , Teléfono: %s    , Sueldo: %.1f    \n", emp.clave,emp.nombre, emp.telefono, emp.sueldo);
    }
    close(fd_index);
}


int main(){
    
    CrearArchivoIndice();
    Ordenar();
    ImprimirTabla();
    return 0;
}