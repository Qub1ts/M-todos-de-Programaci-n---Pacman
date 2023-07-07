#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// Función para crear una matriz de punteros
char** crearMatriz(int filas, int columnas) {
    char** matriz = malloc(filas * sizeof(char*));
    for (int i = 0; i < filas; i++) {
        matriz[i] = malloc(columnas * sizeof(char));
    }
    return matriz;
}

FILE* abrirArchivo(char *nombreFile) {
    FILE *archivo;
    archivo = fopen(nombreFile,"r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n",nombreFile);
        return NULL;
    }
    printf("El archivo %s se abre correctamente\n",nombreFile);
    return archivo;
}

void llenar(char** matriz,int filas,int columnas,FILE* fp) {
    for (int i = 0;i < filas;i++) {
        for (int j = 0;j < columnas;j++) {
            matriz[i][j] = fgetc(fp);
        }
        int caracter = fgetc(fp);
    }
}

void printMatriz(char**matriz,int filas,int columnas) {
    for (int i = 0;i < filas;i++) {
        for (int j = 0;j < columnas;j++) {
            printf("%c",matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {

    char** matriz = crearMatriz(20,17);

    FILE* fp = abrirArchivo("laberinto.txt");

    int filas,columnas,guindas;
    char caracter;

    // Obtiene las filas,columnas y guindas del archivo
    fscanf(fp,"%d %d %d",&filas,&columnas,&guindas);

    caracter = fgetc(fp); // OBTIENE SALTO DE LINEA 

    llenar(matriz,20,17,fp);
    

    

    printMatriz(matriz,20,17);

    


    return 0;
}