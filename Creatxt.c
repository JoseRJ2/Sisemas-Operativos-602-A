#include <stdio.h>

int main() {
    FILE *archivo; // Puntero al archivo

    // Abrir el archivo en modo escritura ("w")
    archivo = fopen("archivo.txt", "w");

    // Verificar si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1; // Salir del programa con código de error
    }

    // Escribir en el archivo
    fprintf(archivo, "Hola, mundo!\n");

    // Cerrar el archivo
    fclose(archivo);

    printf("Se ha escrito en el archivo correctamente.\n");

    return 0;
}
