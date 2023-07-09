#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

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
    char letra;
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
    double gameTime;
}matchData;

// ---------------------------------------------------Funciones--------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------------- //
/*
Funcion: crearMatriz
Entrada: 
Descripcion:
Salida:
*/
char** crearMatriz(int filas, int columnas);

/* 
Funcion: copiarMatriz 
Entrada:  
Descripcion:
Salida:
*/
char** copiarMatriz(int filas,int columnas,char** matrizOg);

/*
Funcion: initializeMatchStats
Entrada:  
Descripcion:
Salida:
*/
void initializeMatchStats(matchData* match,int id);

/*
Funcion: delimitarLaberinto
Entrada:  
Descripcion:
Salida:
*/
void delimitarLaberinto(int filas,int columnas,char** laberint);

/*
Funcion: obtenerLaberinto
Entrada:  
Descripcion:
Salida:
*/
void obtenerLaberinto(FILE* fp,int filas,int columnas,char** laberint,cord* pacmanSpawn,cord* ghostSpawn,cord* pasillo1,cord* pasillo2,int* conteoPuntitosEnMapa);

/*
Funcion: imprimirLaberinto
Entrada:  
Descripcion:
Salida:
*/
void imprimirLaberinto(int filas, int columnas,char** laberint,int namnam);

/*
Funcion: initializePacman
Entrada:  
Descripcion:
Salida:
*/
void initializePacman(pacman* pacman,cord* pacmanSpawn,int vidas);

/*
Funcion: initializeGhost
Entrada:  
Descripcion:
Salida:
*/
void initializeGhost(ghost* ghost,int spawnSpaces,time_t spawnTimer,cord ghostSpawn);

/*
Funcion: letGhostSpawn
Entrada:  
Descripcion:
Salida:
*/
void letGhostSpawn(ghost* ghost,char** maze);

/*
Funcion: pacmanMovement
Entrada:  
Descripcion:
Salida:
*/
void pacmanMovement(pacman* pacman,char** maze,int* conteoPuntitosAux,int* namnam,int* segundosParaComer,time_t* timeToEat,cord* pasillo1,cord* pasillo2,cord* ghostSpawn,ghost* ghost1,ghost* ghost2,ghost* ghost3,ghost* ghost4,int* muertePacman,matchData* match);

/*
Funcion: ghostMovement
Entrada:  
Descripcion:
Salida:
*/
void ghostMovement(ghost* ghost,pacman* pacman,char** maze,cord* pasillo1,cord* pasillo2,cord* ghostSpawn,int* muertePacman,int namnam,matchData* match);

/*
Funcion: spawnGuindas
Entrada:  
Descripcion:
Salida:
*/
void spawnGuindas(time_t* startGuindasTime,int guindasTimeSpawn,cord* pacmanSpawn,char** maze,int* segundosParaComer,ghost* ghost1,ghost* ghost2,ghost* ghost3,ghost* ghost4);

/*
Funcion: spawnPlatanos
Entrada:  
Descripcion:
Salida:
*/
void spawnPlatanos(time_t* startPlatanoTime,int platanoTimeSpawn,cord* pacmanSpawn,char** maze,ghost* ghost1,ghost* ghost2,ghost* ghost3,ghost* ghost4,cord* ghostSpawn);

/*
Funcion: userInput
Entrada:  
Descripcion:
Salida:
*/
void userInput(pacman* pacman);

/*
Funcion: parpadeoTablero
Entrada:  
Descripcion:
Salida:
*/
void parpadeoTablero(int filas,int columnas,char** laberint,int param,pacman* pacman);

/*
Funcion: obtenerFechaActual
Entrada:  
Descripcion:
Salida:
*/
void obtenerFechaActual(char* fecha);

/*
Funcion: printStats
Entrada:  
Descripcion:
Salida:
*/
void printStats(matchData* match,double totalGameTime);

/*
Funcion: generateStatsFile
Entrada:  
Descripcion:
Salida:
*/
void generateStatsFile(matchData* match,double totalGameTime);