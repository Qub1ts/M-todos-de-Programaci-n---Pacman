#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "pacmanFunciones.h"

// Definición de colores
#define BLUE 1
#define RED 4
#define GREEN 10
#define YELLOW 6
#define PURPLE 13
#define WHITE 15


// IMPRIMIR LABERINTO
void imprimirLaberinto(int filas, int columnas,char** laberint,int namnam) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Conecta el codigo con la consola
    for (int i=0;i<filas;i++){
        for (int j=0;j<columnas;j++){
            if (laberint[i][j] == 'C' || laberint[i][j] == '.' || laberint[i][j] == 'o') {
                SetConsoleTextAttribute(hConsole,YELLOW);
                printf("%c",laberint[i][j]);
                SetConsoleTextAttribute(hConsole,WHITE);
            } else if (laberint[i][j] == '#') {
                SetConsoleTextAttribute(hConsole,BLUE);
                printf("%c",laberint[i][j]);
                SetConsoleTextAttribute(hConsole,WHITE);
            } else if (laberint[i][j] == 'B' || laberint[i][j] == 'M' || laberint[i][j] == 'K' || laberint[i][j] == 'E') {
                if (namnam == 0) {
                    SetConsoleTextAttribute(hConsole,PURPLE);
                    printf("%c",laberint[i][j]);
                    SetConsoleTextAttribute(hConsole,WHITE);
                } else {
                    SetConsoleTextAttribute(hConsole,WHITE);
                    printf("%c",laberint[i][j]);
                }
            } else if (laberint[i][j] == '6') {
                SetConsoleTextAttribute(hConsole,RED);
                printf("%c",laberint[i][j]);
                SetConsoleTextAttribute(hConsole,WHITE);
            } else if (laberint[i][j] == 'J') {
                SetConsoleTextAttribute(hConsole,GREEN);
                printf("%c",laberint[i][j]);
                SetConsoleTextAttribute(hConsole,WHITE);
            } else {
                printf("%c",laberint[i][j]);
            }
        }
        printf("\n");
    }
}

// DELIMITAR LABERINTO
void delimitarLaberinto(int filas,int columnas,char** laberint) {
    // Delimitar filas
    for (int j = 0; j < columnas;j++){
        laberint[0][j] = '#';
    }
    for (int j = 0; j < columnas;j++){
        laberint[filas-1][j] = '#';
    }

    // Delimitar columnas
    for (int i = 0; i < filas;i++){
        laberint[i][0] = '#';
    }
    for (int i = 0; i < filas;i++){
        laberint[i][columnas-1] = '#';
    }
}

// OBTIENE LOS CARACTERES DEL LABERINTO Y LO ADAPTA. GUARDA COORDENADAS RELEVANTES
void obtenerLaberinto(FILE* fp,int filas,int columnas,char** laberint,cord* pacmanSpawn,cord* ghostSpawn,cord* pasillo1,cord* pasillo2,int* conteoPuntitosEnMapa) {
    char caracter;
    for (int i=1;i<filas-1;i++){
        for (int j=1;j<columnas-1;j++){
            caracter = fgetc(fp); //file get caracter
            if (caracter == 'x') {
                laberint[i][j] = '.';
                (*conteoPuntitosEnMapa)++;
            } else if (caracter == 'X') {
                laberint[i][j] = 'o';
                (*conteoPuntitosEnMapa)++;
            } else if (caracter == 'w') {
                laberint[i][j] = '#';
            // Pasillo    
            } else if (caracter == 'P') {
                laberint[i][j] = ' ';
                if (j == 17) {
                    laberint[i][18] = ' ';
                    pasillo2->x = i;
                    pasillo2->y = 18;
                } 
                if (j == 1) {
                    laberint[i][0] = ' ';
                    pasillo1->x = i;
                    pasillo1->y = 0;
                }
            } else if (caracter == 'p') {
                laberint[i][j] = 'C';
                pacmanSpawn->x = i; // Se guardan las coordenadas del punto de aparición de Pacman
                pacmanSpawn->y = j;
            } else if (caracter == 'S') {
                laberint[i][j] = ' ';
                ghostSpawn->x = i; // Se guardan las coordenadas del punto de aparición de los fantasmas
                ghostSpawn->y = j;
            } else if (caracter == ' ') {
                laberint[i][j] = ' ';
            }
        }
        caracter = fgetc(fp); // OBTIENE SALTO DE LINEA
    }
    // Cerrar el archivo fuente
    fclose(fp);
}

// Función para crear una matriz de punteros
char** crearMatriz(int filas, int columnas) {
    char** matriz = malloc(filas * sizeof(char*));
    for (int i = 0; i < filas; i++) {
        matriz[i] = malloc(columnas * sizeof(char));
    }
    return matriz;
}

// POSICIONA LO QUE SE IMPRIME EN CONSOLA
void setCursorPosition(int x, int y) {
    COORD coordinates = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

// COPIAR MATRIZ
char** copiarMatriz(int filas,int columnas,char** matrizOg) {
    char** newMatriz = crearMatriz(filas,columnas);
    for (int i = 0;i < filas;i++) {
        for (int j = 0;j < columnas;j++) {
            newMatriz[i][j] = matrizOg[i][j];
        }
    }
    return newMatriz;
}

// PARPADEO DE TABLERO AL MORIR PACMAN
void parpadeoTablero(int filas,int columnas,char** laberint,int param,pacman* pacman) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,GREEN);printf("Score: %d   Vidas: %d\n",pacman->score,pacman->vidas);SetConsoleTextAttribute(hConsole,WHITE);
    imprimirLaberinto(filas,columnas,laberint,param);
    Sleep(300);system("cls");Sleep(300);
    SetConsoleTextAttribute(hConsole,GREEN);printf("Score: %d   Vidas: %d\n",pacman->score,pacman->vidas);SetConsoleTextAttribute(hConsole,WHITE);
    imprimirLaberinto(filas,columnas,laberint,param);
    Sleep(300);system("cls");Sleep(300);
    SetConsoleTextAttribute(hConsole,GREEN);printf("Score: %d   Vidas: %d\n",pacman->score,pacman->vidas);SetConsoleTextAttribute(hConsole,WHITE);
    imprimirLaberinto(filas,columnas,laberint,param);
    Sleep(300);system("cls");Sleep(300);
}



// INICIALIZAR PACMAN
void initializePacman(pacman* pacman,cord* pacmanSpawn,int vidas) {
    pacman->coordenadas = *pacmanSpawn; // Coordenadas iniciales
    pacman->score = 0;
    pacman->vidas = vidas;
    pacman->vx = 0;
    pacman->vy = 0;
}

// INICIALIZAR FANTASMA
void initializeGhost(ghost* ghost,int spawnSpaces,time_t spawnTimer,cord ghostSpawn) {
    ghost->spawned = 0;
    ghost->spawnTimer = spawnTimer;
    ghost->coordenadas = ghostSpawn;
    ghost->comido = ' ';
    ghost->vx = -1;
    ghost->vy = 0;
    ghost->spawnSpaces = spawnSpaces;
}

// OBTIENE EL INPUT DEL USUARIO PARA CONTROLAR A PACMAN
void userInput(pacman* pacman) {
    // Recibe las teclas que presiona el usuario para controlar a Pacman
    if (_kbhit()) {
        char ch = _getch();
        pacman->vx = 0; // Se reinicia el movimiento de Pacman
        pacman->vy = 0; // Se reinicia el movimiento de Pacman
        // a = 97; d = 100; w = 119; s = 115
        switch(ch) {
            case 97: // A
                pacman->vy = -1;
                break;
            case 100: // D
                pacman->vy = 1;
                break;
            case 119: // W
                pacman->vx = -1;
                Sleep(50);
                break;
            case 115: // S
                pacman->vx = 1;
                Sleep(50);
                break;
        }
    }
}

