#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// ---------------------------------------------------Estructuras------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------------- //

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

typedef struct fantasma{
    cord coordenadas;
    int vx;
    int vy;
    int spawned;
    char comido;
    int spawnSpaces; // Iteraciones para spawnear
    time_t spawnTimer; // Temporizador de spawneo
}ghost;

typedef struct datosDePartida{
    int id;
    int scoreFinal;
    int fantasmasComidos;
    int muertes;
    int smallDots;
    int bigDots;
}matchData;

// ---------------------------------------------------Funciones------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------------- //


void imprimirLaberinto(int filas, int columnas,char** laberint,int namnam);

void delimitarLaberinto(int filas,int columnas,char** laberint);

void obtenerLaberinto(FILE* fp,int filas,int columnas,char** laberint,cord* pacmanSpawn,cord* ghostSpawn,cord* pasillo1,cord* pasillo2,int* conteoPuntitosEnMapa);

char** crearMatriz(int filas, int columnas);

void setCursorPosition(int x, int y);

char** copiarMatriz(int filas,int columnas,char** matrizOg);

void parpadeoTablero(int filas,int columnas,char** laberint,int param,pacman* pacman);

void initializePacman(pacman* pacman,cord* pacmanSpawn,int vidas);

void initializeGhost(ghost* ghost,int spawnSpaces,time_t spawnTimer,cord ghostSpawn);

void userInput(pacman* pacman);



