#include <stdio.h>
#include <stdlib.h>

void imprimirMatriz(int fil,int colum,int matrix[fil][colum]) {
    for (int i = 0;i < fil;i++) {
        for (int j = 0;j < colum;j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}


int main() {

    int filas = 5;
    int columnas = 8;

    int matriz[filas][columnas];

    for (int i = 0;i < filas;i++) {
        for (int j = 0;j < columnas;j++) {
            matriz[i][j] = i + j;
        }
    }

    imprimirMatriz(filas,columnas,matriz);

    return 0;
}