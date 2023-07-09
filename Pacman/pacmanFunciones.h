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
Entrada: Filas y columnas con numero entero
Descripcion: Crear la matriz del laberinto como puntero asignandole memoria con la funcion malloc()     
Salida: Devuelve una matriz del orden que queramos en este caso 20x17
*/
char** crearMatriz(int filas, int columnas);

/* 
Funcion: copiarMatriz 
Entrada: Filas y columnas con numero entero mas un puntero de matriz del tipo char que es la matriz original
Descripcion: Se crea esta copia de la matriz original para hacer todos los cambios en ella sin afectar a la original
Salida: Matriz del tamaño que queramos
*/
char** copiarMatriz(int filas,int columnas,char** matrizOg);

/*
Funcion: initializeMatchStats
Entrada: Estructura matchData y hacer puntero match y un entero id
Descripcion: Inicializar el score en 0  
Salida: Estadisticas en  0
*/
void initializeMatchStats(matchData* match,int id);

/*
Funcion: delimitarLaberinto
Entrada:  Filas y columnas del tipo entero y un puntero del laberinto original
Descripcion: Delimita en filas y columnas por arriba y poner un # como pared por la parte superior [0][j] por la  inferior [i-1][j] por los costados [i][0] y por la derecha [i][j-1]
Salida: Rellena con # todo el borde del laberinto
*/
void delimitarLaberinto(int filas,int columnas,char** laberint);

/*
Funcion: obtenerLaberinto
Entrada:Archivo .txt, fila y coluumnas de tipo entero, la matriz del laberinto, las cordenasdas de respawn del pacman, los fantasmas, los pasillos y un puntero para el conteo de puntitos
Descripcion: Genera el mapa recibido .txt pero con las modificaciones respectivas como el cambio de simbolo de las paredes, puntitos.
Salida: Genera el nuevo mapa con los simbolos significativos y posiciona los respawn del pacman, fantasma, pasillos.
*/
void obtenerLaberinto(FILE* fp,int filas,int columnas,char** laberint,cord* pacmanSpawn,cord* ghostSpawn,cord* pasillo1,cord* pasillo2,int* conteoPuntitosEnMapa);

/*
Funcion: imprimirLaberinto
Entrada: Filas y columnas del tipo entero, el laberinto original y una variable del tipo entero namnam 
Descripcion: Imprimer el laberinto con los respectivos colores y con la funcion de namnam poder decir si es 0 los fantasmas no se puden comer y estan de color purple y cuando se pueden
// comer es por que es diferente de 1 y los fantasmas estan de color blanco
Salida: Imprimer el laberinto a colores para identificar y visualizar mejor el mapa
*/
void imprimirLaberinto(int filas, int columnas,char** laberint,int namnam);

/*
Funcion: initializePacman
Entrada:  Estructura pacman, las cordenadas del respawn del pacman y una variable para las vidas del tipo entero
Descripcion: Define todas las variables del pacman con la estructura declarada definiendo su punto inicial, las vidas en 0, y las direccion a las que se puede mover 
Salida: Inicializa el pacman con las condiciones inciales 
*/
void initializePacman(pacman* pacman,cord* pacmanSpawn,int vidas);

/*
Funcion: initializeGhost
Entrada: estructura ghost, una variable del tipo entero spawnspace, time_t para llevar el tiempo en cuanto respawnear y las cordenadas del respawn de los fantasmas
Descripcion: Se definen todas las variables que caracterizan a los fantasmas cuando puden ser comido o cuando no, cuanto tiempo tienen que respawnear cada uno
Salida: Inicializa a los fantasmas con las condiciones iniciales
*/
void initializeGhost(ghost* ghost,int spawnSpaces,time_t spawnTimer,cord ghostSpawn);

/*
Funcion: letGhostSpawn
Entrada: Estructura de ghost y un puntero del laberinto llamdado maze
Descripcion: Permitiremos respawnear a los fantasmas una vez que el pacman se lo haya comido o en la condicion inicial
Salida: Localiza donde respawnear a los fantasmas
*/
void letGhostSpawn(ghost* ghost,char** maze);

/*
Funcion: pacmanMovement
Entrada: Estructura del pacman, un puntero del lebarinto, puntero para el conteo de puntos aux, puntero de namnam, puntero de los segundos para comer, las cordenadas de los pasillos,
//la cordenadas de respawn de los fantasmas, puntero de las muertes, y una estructura matchdata
Descripcion: Incializar al pacman para que cumpla todas las funcionalidades de comer a los fantasmas, comero los puntos amarillos grandes y chicos, comer las frutas que van respawneando 
//en su condicion inicial, tambien poder trasladarse por los pasillos
Salida: Pacman incializado con todas las funcionalidades de movimientos e interaccion con todo lo que hay en el laberinto
*/
void pacmanMovement(pacman* pacman,char** maze,int* conteoPuntitosAux,int* namnam,int* segundosParaComer,time_t* timeToEat,cord* pasillo1,cord* pasillo2,cord* ghostSpawn,ghost* ghost1,ghost* ghost2,ghost* ghost3,ghost* ghost4,int* muertePacman,matchData* match);

/*
Funcion: ghostMovement
Entrada: estructura de fantasmas, estructura del pacman, un puntero del laberinto, las cordenadas de los pasillos, cordenada del respawn de los fantasmas, puntero de las muertes de pacman
// variable del tipo entero namnam y la estructura matchdata 
Descripcion: Incializa a cada uno de los fantasmas poder interacturar con todo lo que hay en el laberinto evitar comerse puntos y traspasarse entre si, tener la funcionalidad de comer
//al pacman y de aparecer cuando el pacman se come a este
Salida: Fantasmas inicializados con todas las funcionalidades de movimientoo e interaccion con todos los objetos que hat en el laberinto
*/
void ghostMovement(ghost* ghost,pacman* pacman,char** maze,cord* pasillo1,cord* pasillo2,cord* ghostSpawn,int* muertePacman,int namnam,matchData* match);

/*
Funcion: spawnGuindas
Entrada: Time_t funcion de libreria time.h, variable del tipo entero para las guindas en el respawn, estructuras de cordenadas del pacman, el puntero del laberinto, un puntero para los
//segundos para comer y la estructura de los fantasmas
Descripcion: Respawnea las guindas y nos da 2 segundos mas para poder comer a los fantasmas con la funcion time_t podemos iniciar el tiempo en segundos.
Salida: Respawn de las guindas y la funcion particular que cumple
*/
void spawnGuindas(time_t* startGuindasTime,int guindasTimeSpawn,cord* pacmanSpawn,char** maze,int* segundosParaComer,ghost* ghost1,ghost* ghost2,ghost* ghost3,ghost* ghost4);

/*
Funcion: spawnPlatanos
Entrada: Time_t funcion de libreria time.h, variable del tipo entero para el respawn del platano, estructuras de cordenadas del pacman, el puntero del laberinto, la estructura de los
// fantasmas y las cordenadas del respawn de los fantasmas 
Descripcion: respawnea los platanos y nos da la habilidad de que todos los fantasmas vuelvan a su base y determinando los espacios para poder salir el siguiente fantasma
Salida: Respawn de los platanos y la funcion particular que cumple
*/
void spawnPlatanos(time_t* startPlatanoTime,int platanoTimeSpawn,cord* pacmanSpawn,char** maze,ghost* ghost1,ghost* ghost2,ghost* ghost3,ghost* ghost4,cord* ghostSpawn);

/*
Funcion: userInput
Entrada: estructura del pacman
Descripcion: con la funcion _kbhit() podemos analizar si se esta detectando el teclado con la norma ASCII y con la funcion _getch se lee el caracter elegido y con una estructura de
// control poder verificar que tecla se apreto
Salida: Deteccion del movimiento que quiere hacer el usuario
*/
void userInput(pacman* pacman);

/*
Funcion: parpadeoTablero
Entrada: filas y columnas del tipo entero, un puntero del laberinto original, una variable entero parm y la estructura del pacman
Descripcion: Se define para que no parpede la pantalla cada vez que el pacman muere y asi tener una animacion mas dinamica, accediendo a la consola y se llama a la funcion de 
//imprimirlaberinto y cambiandole el color.
Salida: Hace un efecto de parpadeo al momento de morir el pacman
*/
void parpadeoTablero(int filas,int columnas,char** laberint,int param,pacman* pacman);

/*
Funcion: obtenerFechaActual
Entrada:  puntero con un solo caracter
Descripcion: obtenenemos la fecha actual en forma de string
Salida: fecha actual en forma de string
*/
void obtenerFechaActual(char* fecha);

/*
Funcion: printStats
Entrada: estructura de los datos de la partida, un variable double del tiempo total del juego
Descripcion: Se imprimen las estadisticas de la partida gracias a los datos entregados de la estructura
Salida: Imprime las estadistica de la partida
*/
void printStats(matchData* match,double totalGameTime);

/*
Funcion: generateStatsFile
Entrada: Estructura matchdata y una variable para el tiempo total del juego del tipo double
Descripcion: Genera un archivo de salida .out con todas las estadisticas de la partida jugada recientemente
Salida: Genera el archivo de salida .out
*/
void generateStatsFile(matchData* match,double totalGameTime);