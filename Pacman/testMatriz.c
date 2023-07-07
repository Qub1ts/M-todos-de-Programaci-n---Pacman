#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

void xd(int matrix[][]) {
    
    int k = 2;
    int m = 5;
    matrix[k][m] = 29;
    
}

int main() {

    int filas = 3;
    int columnas = 6;

    int matriz[filas][columnas];

    xd(matriz);

    printf("%d",matriz[2][5]);




    return 0;
}