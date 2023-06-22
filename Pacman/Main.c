#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

typedef struct coord{
    int x;
    int y;
}cord;

typedef struct pacman{
    cord coordenadas;
    int vidas;
    int score;
    int vx;
    int vy;
}pacman;

// ABRIR UN ARCHIVO EN MODO LECTURA
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

void setCursorPosition(int x, int y) {
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

void userInput(){



}

int main() {

    // ABRIR EL ARCHIVO
    FILE* fp = abrirArchivo("laberINTO.txt");

    int filas,columnas,guindas;
    char caracter;

    fscanf(fp,"%d %d %d",&filas,&columnas,&guindas);
    char laberinto[filas+2][columnas+2];

    caracter = fgetc(fp); // OBTIENE SALTO DE LINEA 

    cord pacmanSpawn;
    cord ghostSpawn;

    // Delimitar el laberinto

    // Delimitar filas
    for (int j = 0; j < columnas+2;j++){
        laberinto[0][j] = '#';
    }
    for (int j = 0; j < columnas+2;j++){
        laberinto[filas+1][j] = '#';
    }

    // Delimitar columnas
    for (int i = 0; i < filas+2;i++){
        laberinto[i][0] = '#';
    }
    for (int i = 0; i < filas+2;i++){
        laberinto[i][columnas+1] = '#';
    }

    for (int i=1;i<filas+1;i++){
        for (int j=1;j<columnas+1;j++){
            caracter = fgetc(fp);
            if (caracter == 'x') {
                laberinto[i][j] = '.';
            } else if (caracter == 'X') {
                laberinto[i][j] = 'o';
            } else if (caracter == 'w') {
                laberinto[i][j] = '#';
            } else if (caracter == 'P') {
                laberinto[i][j] = '*';
            } else if (caracter == 'p') {
                laberinto[i][j] = 'C';
                pacmanSpawn.x = i;
                pacmanSpawn.y = j;
            } else if (caracter == 'S') {
                laberinto[i][j] = 'F';
                ghostSpawn.x = i;
                ghostSpawn.y = j;
            } else if (caracter == ' ') {
                laberinto[i][j] = ' ';
            }
        }
        caracter = fgetc(fp); // OBTIENE SALTO DE LINEA
    }

    for (int i=0;i<filas+2;i++){
        for (int j=0;j<columnas+2;j++){
            printf("%c",laberinto[i][j]);
        }
       printf("\n");
    }
    fclose(fp);

    /*

    // INICIALIZANDO PACMAN
    pacman pacmanX;
    pacmanX.coordenadas = pacmanSpawn; // Coordenadas iniciales
    pacmanX.score = 0;
    pacmanX.vidas = 3;
    pacmanX.vx = 0;
    pacmanX.vy = 0;

    // INICIALIZANDO FANTASMAS

    // USER INPUT

    char ch;

    while (1) {
        system("cls");

        if (_kbhit()) {
            ch = _getch();
            // a = 97;d = 100;w = 119;s = 115
            switch(ch) {
                case 97:
                    pacmanX.vy = -1;
                    break;
                case 100:
                    pacmanX.vy = 1;
                    break;
                case 119:
                    pacmanX.vx = -1;
                    break;
                case 115:
                    pacmanX.vx = 1;
                    break;

                }
            }

        // MOVIMIENTO
        int lx,ly;
        lx = pacmanX.coordenadas.x + pacmanX.vx;
        ly = pacmanX.coordenadas.y + pacmanX.vy;

        if (laberinto[lx][ly] == '#') {
            pacmanX.vx = 0;
            pacmanX.vy = 0;
        } else if (laberinto[lx][ly] == '.') {
            pacmanX.score += 3;
            laberinto[lx][ly] = 'C';
            laberinto[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
            pacmanX.coordenadas.x = lx;
            pacmanX.coordenadas.y = ly;
        } else if (laberinto[lx][ly] == 'o') {
            pacmanX.score += 7;
            laberinto[lx][ly] = 'C';
            laberinto[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
            pacmanX.coordenadas.x = lx;
            pacmanX.coordenadas.y = ly;
        } else if (laberinto[lx][ly] == '*') {
            pacmanX.score += 7;
            laberinto[lx][ly] = 'C';
            laberinto[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
            pacmanX.coordenadas.x = lx;
            pacmanX.coordenadas.y = ly;
        } else if (laberinto[lx][ly] == 'F') {
            pacmanX.score += 7;
            laberinto[lx][ly] = 'C';
            laberinto[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
            pacmanX.coordenadas.x = lx;
            pacmanX.coordenadas.y = ly;
        } else if (laberinto[lx][ly] == ' ') {
            laberinto[lx][ly] = 'C';
            laberinto[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
            pacmanX.coordenadas.x = lx;
            pacmanX.coordenadas.y = ly;
        }

        // IMPRIMIR LABERINTO
        for (int i=0;i<filas+2;i++){
        for (int j=0;j<columnas+2;j++){
            printf("%c",laberinto[i][j]);
        }
        printf("\n");
        }
        printf("Score: %d",pacmanX.score);

        Sleep(30);

        //setCursorPosition(0,0);
    }

    */

    return 0;
}