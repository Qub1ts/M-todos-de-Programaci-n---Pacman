#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// Definición de colores
#define BLUE 1
#define RED 4
#define GREEN 10
#define YELLOW 6
#define PURPLE 13
#define WHITE 15

//GameOver
char gameOver[8][90] = {"  /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$        /$$$$$$  /$$    /$$ /$$$$$$$$ /$$$$$$$ ",
                        " /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/       /$$__  $$| $$   | $$| $$_____/| $$__  $$",
                        "| $$  \\__/| $$  \\ $$| $$$$  /$$$$| $$            | $$  \\ $$| $$   | $$| $$      | $$  \\ $$",
                        "| $$ /$$$$| $$$$$$$$| $$ $$/$$ $$| $$$$$         | $$  | $$|  $$ / $$/| $$$$$   | $$$$$$$/",
                        "| $$|_  $$| $$__  $$| $$  $$$| $$| $$__/         | $$  | $$ \\  $$ $$/ | $$__/   | $$__  $$",
                        "| $$  \\ $$| $$  | $$| $$\\  $ | $$| $$            | $$  | $$  \\  $$$/  | $$      | $$  \\ $$",
                        "|  $$$$$$/| $$  | $$| $$ \\/  | $$| $$$$$$$$      |  $$$$$$/   \\  $/   | $$$$$$$$| $$  | $$",
                        " \\______/ |__/  |__/|__/     |__/|________/       \\______/     \\_/    |________/|__/  |__/"
};

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

// ----------------------------------------------------Funciones-------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------------- //

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

// IMPRIMIR LABERINTO
void imprimirLaberinto(int filas, int columnas,char laberint[filas][columnas],int namnam) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
            } else {
                printf("%c",laberint[i][j]);
            }
        }
        printf("\n");
    }
}

// DELIMITAR LABERINTO
void delimitarLaberinto(int filas,int columnas,char laberint[filas][columnas]) {
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
void obtenerLaberinto(FILE* fp,int filas,int columnas,char laberint[filas][columnas],cord* pacmanSpawn,cord* ghostSpawn,cord* pasillo1,cord* pasillo2,int* conteoPuntitosEnMapa) {
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

// POSICIONA LO QUE SE IMPRIME EN CONSOLA
void setCursorPosition(int x, int y) {
    COORD coordinates = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

// COPIAR MATRIZ
void copiarMatriz(int filas,int columnas,char matrizOg[filas][columnas],char matrizResultante[filas][columnas]) {
    for (int i = 0;i < filas;i++) {
        for (int j = 0;j < columnas;j++) {
            matrizResultante[i][j] = matrizOg[i][j];
        }
    }
}

void parpadeoTablero(int filas,int columnas,char laberint[filas][columnas],int param,pacman* pacman) {
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

// IMPRIMIR GAMEOVER
void printGameOver() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,RED);
    for (int i = 0;i < 8;i++) {
        for (int j = 0;j < 90;j++) {
            printf("%c",gameOver[i][j]);
        }
        printf("\n"); 
    }
    SetConsoleTextAttribute(hConsole,WHITE);
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

// ------------------------------------------------------Main----------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------------- //

// - gcc main.c header.c
// - a.exe laberinto.txt

int main(int argc, char* argv[]) {

    // ABRIR EL ARCHIVO
    FILE* fp = abrirArchivo(argv[1]);

    int filas,columnas,guindas;
    char caracter;

    // Obtiene las filas,columnas y guindas del archivo
    fscanf(fp,"%d %d %d",&filas,&columnas,&guindas);

    // Crea laberinto a partir de las filas y columnas obtenidas del archivo
    char laberinto[filas+2][columnas+2];

    caracter = fgetc(fp); // OBTIENE SALTO DE LINEA 

    // Coordenadas del punto de aparición de Pacman en el laberinto
    cord pacmanSpawn;
    // Coordenadas del punto de aparición de los fantasmas en el laberinto
    cord ghostSpawn;
    // Coordenadas del pasillo 1
    cord pasillo1;
    // Coordenadas del pasillo 2
    cord pasillo2;

    // Conteo de los puntos totales que estan esparcidos en el laberinto
    int conteoPuntitosEnMapa = 0;

    // Delimitar el laberinto
    delimitarLaberinto(filas+2,columnas+2,laberinto);

    // Inicializar laberinto
    // Obtiene los carácters del archivo
    obtenerLaberinto(fp,filas+2,columnas+2,laberinto,&pacmanSpawn,&ghostSpawn,&pasillo1,&pasillo2,&conteoPuntitosEnMapa);

    // Creacion de laberinto auxiliar el cual sufrirá los cambios durante el juego, para preservar asi el original intacto
    char laberintoAux[filas+2][columnas+2];

    // Creacion de entidades del juego
    pacman pacmanX;
    ghost blinky;
    ghost pinky;
    ghost inky;
    ghost clyde;

    int salirDelJuego = 0;

    //-------------------------------------------------SALIR DEL JUEGO----------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------------------------------------//
        
    // La opción de salir del juego estará disponible cada vez que Pacman gane o pierda sus 3 vidas.
    // Se le preguntará al usuario si desea volver a jugar.
    while (salirDelJuego != 1) {

        copiarMatriz(filas+2,columnas+2,laberinto,laberintoAux);

        //--------------------------------------------INICIALIZANDO PACMAN----------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//
        initializePacman(&pacmanX,&pacmanSpawn,3);

        //-------------------------------------------INICIALIZANDO FANTASMAS--------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//
    
        initializeGhost(&blinky,1,10,ghostSpawn); // B
        initializeGhost(&pinky,7,10,ghostSpawn);  // M
        initializeGhost(&inky,13,10,ghostSpawn);  // K
        initializeGhost(&clyde,19,10,ghostSpawn); // E

        //-------------------------------------------INICIALIZANDO VARIABLES DE JUEGO-----------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//

        int finDelJuego = 0;
        // Flag que indica si Pacman murio 
        int muertePacman = 0;
        int conteoPuntitosAux = conteoPuntitosEnMapa;
        double tiempoDeJuego;
        double tiempoDeComida;
        double tiempoDeGuinda;
        // Es la frecuencia de tiempo en la cual spawnearan guindas
        int guindasTimeSpawn = (180/guindas) - 15;
        // Flag que indica si Pacman puede comerse a los fantasmas
        int namnam = 0;
        int segundosParaComer = 8;
        time_t timeToEat = 10;
        time_t currentGameTime;

        time_t startGameTime = time(NULL);
        time_t startGuindasTime = time(NULL);

        // Limpia la consola de comandos
        system("cls");

        //-----------------------------------------------------FIN DEL JUEGO--------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//

        // Mientras el flag de "finDeJuego" sea distinto de 1, es decir, False, el juego sigue. 
        while (finDelJuego != 1) {

            // Verifica si Pacman ha muerto para ver si se reinicia el tablero
            if (muertePacman == 1) {
                // Se reinicia el tablero
                copiarMatriz(filas+2,columnas+2,laberinto,laberintoAux); 
                // Se reinicia Pacman
                initializePacman(&pacmanX,&pacmanSpawn,pacmanX.vidas-1); 
                // Se reinician los Fantasmas
                initializeGhost(&blinky,1,10,ghostSpawn);
                initializeGhost(&pinky,7,10,ghostSpawn);
                initializeGhost(&inky,13,10,ghostSpawn);
                initializeGhost(&clyde,19,10,ghostSpawn);

                time_t timeToEat = 10;
                namnam = 0;
                segundosParaComer = 8;
            
                conteoPuntitosAux = conteoPuntitosEnMapa; // Reinicio de la cuenta de puntitos comidos.

                // PARPADEO DE TABLERO
                if (pacmanX.vidas != 0) {
                    parpadeoTablero(filas+2,columnas+2,laberintoAux,namnam,&pacmanX);
                }

                // Reinicio de temporizador de juego
                startGameTime = time(NULL);
                startGuindasTime = time(NULL);

                muertePacman = 0;
            }

            //--------------------------------------------------------USER INPUT--------------------------------------------------------------//
            //--------------------------------------------------------------------------------------------------------------------------------//
            
            // Recibe las teclas que presiona el usuario para controlar a Pacman
            userInput(&pacmanX);
                
            //---------------------------------------------------MOVIMIENTO PACMAN---------------------------------------------------------//
            //-----------------------------------------------------------------------------------------------------------------------------//

            int lx,ly;
            lx = pacmanX.coordenadas.x + pacmanX.vx;
            ly = pacmanX.coordenadas.y + pacmanX.vy;

            // PARED
            if (laberintoAux[lx][ly] == '#') {
                pacmanX.vx = 0;
                pacmanX.vy = 0;
            // PUNTO NORMAL 3
            } else if (laberintoAux[lx][ly] == '.') {
                pacmanX.score += 3;
                conteoPuntitosAux -= 1;
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
            // PUNTO GRANDE 7
            } else if (laberintoAux[lx][ly] == 'o') {
                pacmanX.score += 7;
                conteoPuntitosAux -= 1;
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
                namnam = 1;
                timeToEat = time(NULL);
            // GUINDA
            } else if (laberintoAux[lx][ly] == '6') {
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
                segundosParaComer += 2;
            // PASILLO 1
            } else if (lx == pasillo1.x && ly == pasillo1.y)  {
                laberintoAux[pasillo2.x][pasillo2.y] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = pasillo2.x;
                pacmanX.coordenadas.y = pasillo2.y;
            // PASILLO 2    
            } else if (lx == pasillo2.x && ly == pasillo2.y)  {
                laberintoAux[pasillo1.x][pasillo1.y] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = pasillo1.x;
                pacmanX.coordenadas.y = pasillo1.y;
            // SPAWN DE FANTASMAS (NO DEBE PODER PASAR)
            } else if (lx == ghostSpawn.x && ly == ghostSpawn.y) {
                pacmanX.vx = 0;
                pacmanX.vy = 0;
            // ESPACIO LIBRE
            } else if (laberintoAux[lx][ly] == ' ') {
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
            // COLISION CON FANTASMAS
            } else if (laberintoAux[lx][ly] == 'B') {
                if (namnam == 0) {
                    laberintoAux[lx][ly] = 'B';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    muertePacman = 1;
                } else {
                    pacmanX.score += 10;
                    if (blinky.comido == '.') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (blinky.comido == 'o') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (blinky.comido == '6') {
                        segundosParaComer += 2;
                    }
                    laberintoAux[lx][ly] = 'C';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    initializeGhost(&blinky,1,time(NULL),ghostSpawn);
                }
            } else if (laberintoAux[lx][ly] == 'M') {
                if (namnam == 0) {
                    laberintoAux[lx][ly] = 'M';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    muertePacman = 1;
                } else {
                    pacmanX.score += 10;
                    if (pinky.comido == '.') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (pinky.comido == 'o') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (pinky.comido == '6') {
                        segundosParaComer += 2;
                    }
                    laberintoAux[lx][ly] = 'C';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    initializeGhost(&pinky,7,time(NULL),ghostSpawn);
                }  
            } else if (laberintoAux[lx][ly] == 'K') {
                if (namnam == 0) {
                    laberintoAux[lx][ly] = 'K';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    muertePacman = 1;
                } else {
                    pacmanX.score += 10;
                    if (inky.comido == '.') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (inky.comido == 'o') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (inky.comido == '6') {
                        segundosParaComer += 2;
                    }
                    laberintoAux[lx][ly] = 'C';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    initializeGhost(&inky,13,time(NULL),ghostSpawn);
                } 
            } else if (laberintoAux[lx][ly] == 'E') {
                if (namnam == 0) {
                    laberintoAux[lx][ly] = 'E';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    muertePacman = 1;
                } else {
                    pacmanX.score += 10;
                    if (clyde.comido == '.') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (clyde.comido == 'o') {
                        pacmanX.score += 3;
                        conteoPuntitosAux -= 1;
                    } else if (clyde.comido == '6') {
                        segundosParaComer += 2;
                    }
                    laberintoAux[lx][ly] = 'C';
                    laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                    pacmanX.coordenadas.x = lx;
                    pacmanX.coordenadas.y = ly;
                    initializeGhost(&clyde,19,time(NULL),ghostSpawn);
                }   
            }

            //-----------------------------------------------------SPAWN FANTASMAS------------------------------------------------------------//
            //--------------------------------------------------------------------------------------------------------------------------------//
            if (blinky.spawned == 0) {
                currentGameTime = time(NULL);
                if (difftime(currentGameTime,blinky.spawnTimer) >= 4) {
                    blinky.spawnSpaces -= 1;
                    if (blinky.spawnSpaces == 0) {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = 'B';
                        blinky.spawned = 1;
                    }
                }   
            }

            if (pinky.spawned == 0) {
                currentGameTime = time(NULL);
                if (difftime(currentGameTime,pinky.spawnTimer)  >= 4) {
                    pinky.spawnSpaces -= 1;
                    if (pinky.spawnSpaces == 0) {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = 'B';
                        pinky.spawned = 1;
                    }
                }   
            }

            if (inky.spawned == 0) {
                currentGameTime = time(NULL);
                if (difftime(currentGameTime,inky.spawnTimer)  >= 4) {
                    inky.spawnSpaces -= 1;
                    if (inky.spawnSpaces == 0) {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = 'B';
                        inky.spawned = 1;
                    }
                }   
            }

            if (clyde.spawned == 0) {
                currentGameTime = time(NULL);
                if (difftime(currentGameTime,clyde.spawnTimer) >= 4) {
                    clyde.spawnSpaces -= 1;
                    if (clyde.spawnSpaces == 0) {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = 'B';
                        clyde.spawned = 1;
                    }
                }   
            }

            //---------------------------------------------------MOVIMIENTO FANTASMAS---------------------------------------------------------//
            //--------------------------------------------------------------------------------------------------------------------------------//ss
                // - Los fantasmas podran bloquearse entre sí, no se podrán atravesar.
                // - Los fantasmas no podrán salir del spawn todos al mismo tiempo, lo que conlleva a realizar un temporizador para cada uno para permitir
                //   que estos vayan spawneando por orden.
                // - Si dos fantasmas van a chocar entre si, solo cambiaran de dirección.
                // - Los fantasmas si podrán atravesar los puntos y guindas.
                // - Colision con Pacman.

            int gx,gy;
            int arrayGhostMov[2] = {1,-1};

            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            
            //----BLINKY----//
            //--------------//
            if (blinky.spawned == 1) {
                gx = blinky.coordenadas.x + blinky.vx;
                gy = blinky.coordenadas.y + blinky.vy;
                // PARED
                if (laberintoAux[gx][gy] == '#' || laberintoAux[gx][gy] == 'M' || laberintoAux[gx][gy] == 'K' || laberintoAux[gx][gy] == 'E') {
                    if (blinky.vx != 0) {
                        blinky.vx = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        blinky.vy = arrayGhostMov[rIndex];
                    } else {
                        blinky.vy = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        blinky.vx = arrayGhostMov[rIndex];
                    }
                // PUNTO NORMAL    
                } else if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = blinky.comido;
                    laberintoAux[gx][gy] = 'B';
                    blinky.comido = '.';
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = blinky.comido;
                    laberintoAux[gx][gy] = 'B';
                    blinky.comido = 'o';
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                // GUINDA
                } else if (laberintoAux[gx][gy] == '6') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = blinky.comido;
                    laberintoAux[gx][gy] = 'B';
                    blinky.comido = '6';
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                // PASILLO 1
                } else if (gx == pasillo1.x && gy == pasillo1.y)  {
                    laberintoAux[pasillo2.x][pasillo2.y] = 'B';
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    blinky.coordenadas.x = pasillo2.x;
                    blinky.coordenadas.y = pasillo2.y;
                // PASILLO 2    
                } else if (gx == pasillo2.x && gy == pasillo2.y)  {
                    laberintoAux[pasillo1.x][pasillo1.y] = 'B';
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    blinky.coordenadas.x = pasillo1.x;
                    blinky.coordenadas.y = pasillo1.y;
                // SPAWN DE FANTASMAS (NO DEBE PODER PASAR)
                } else if (gx == ghostSpawn.x && gy == ghostSpawn.y) {
                    blinky.vx = 0;
                    blinky.vy = -1;
                // ESPACIO LIBRE
                } else if (laberintoAux[gx][gy] == ' ') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = blinky.comido;
                    laberintoAux[gx][gy] = 'B';
                    blinky.comido = ' ';
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                // COLISION CON PACMAN
                } else if (laberintoAux[gx][gy] == 'C') {
                    if (namnam == 0) {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = blinky.comido;
                        laberintoAux[gx][gy] = 'B';
                        blinky.comido = ' ';
                        blinky.coordenadas.x = gx;
                        blinky.coordenadas.y = gy;
                        muertePacman = 1;
                    } else {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = blinky.comido;
                        laberintoAux[gx][gy] = 'C';
                        pacmanX.score += 10;
                        initializeGhost(&blinky,1,time(NULL),ghostSpawn);
                    }  
                }
            }

            //----PINKY----//
            //-------------//
            if (pinky.spawned == 1) {
                gx = pinky.coordenadas.x + pinky.vx;
                gy = pinky.coordenadas.y + pinky.vy;
                // PARED
                if (laberintoAux[gx][gy] == '#' || laberintoAux[gx][gy] == 'B' || laberintoAux[gx][gy] == 'K' || laberintoAux[gx][gy] == 'E') {
                    if (pinky.vx != 0) {
                        pinky.vx = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        pinky.vy = arrayGhostMov[rIndex];
                    } else {
                        pinky.vy = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        pinky.vx = arrayGhostMov[rIndex];
                    }
                // PUNTO NORMAL       
                } else if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = pinky.comido;
                    laberintoAux[gx][gy] = 'M';
                    pinky.comido = '.';
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = pinky.comido;
                    laberintoAux[gx][gy] = 'M';
                    pinky.comido = 'o';
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                // GUINDA
                } else if (laberintoAux[gx][gy] == '6') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = pinky.comido;
                    laberintoAux[gx][gy] = 'M';
                    pinky.comido = '6';
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                // PASILLO 1
                } else if (gx == pasillo1.x && gy == pasillo1.y) {
                    laberintoAux[pasillo2.x][pasillo2.y] = 'M';
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    pinky.coordenadas.x = pasillo2.x;
                    pinky.coordenadas.y = pasillo2.y;
                // PASILLO 2    
                } else if (gx == pasillo2.x && gy == pasillo2.y)  {
                    laberintoAux[pasillo1.x][pasillo1.y] = 'M';
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    pinky.coordenadas.x = pasillo1.x;
                    pinky.coordenadas.y = pasillo1.y;
                // SPAWN DE FANTASMAS (NO DEBE PODER PASAR)
                } else if (gx == ghostSpawn.x && gy == ghostSpawn.y) {
                    pinky.vx = 0;
                    pinky.vy = -1;
                // ESPACIO LIBRE
                } else if (laberintoAux[gx][gy] == ' ') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = pinky.comido;
                    laberintoAux[gx][gy] = 'M';
                    pinky.comido = ' ';
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                // COLISION CON PACMAN
                } else if (laberintoAux[gx][gy] == 'C') {
                    if (namnam == 0) {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = pinky.comido;
                        laberintoAux[gx][gy] = 'M';
                        pinky.comido = ' ';
                        pinky.coordenadas.x = gx;
                        pinky.coordenadas.y = gy;
                        muertePacman = 1;
                    } else {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = pinky.comido;
                        laberintoAux[gx][gy] = 'C';
                        pacmanX.score += 10;
                        initializeGhost(&pinky,7,time(NULL),ghostSpawn);
                    }  
                }
            }

            //----INKY----//
            //------------//
            if (inky.spawned == 1) {
                gx = inky.coordenadas.x + inky.vx;
                gy = inky.coordenadas.y + inky.vy;
                // PARED
                if (laberintoAux[gx][gy] == '#' || laberintoAux[gx][gy] == 'B' || laberintoAux[gx][gy] == 'M' || laberintoAux[gx][gy] == 'E') {
                    if (inky.vx != 0) {
                        inky.vx = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        inky.vy = arrayGhostMov[rIndex];
                    } else {
                        inky.vy = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        inky.vx = arrayGhostMov[rIndex];
                    }
                // PUNTO NORMAL       
                } else if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = inky.comido;
                    laberintoAux[gx][gy] = 'K';
                    inky.comido = '.';
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = inky.comido;
                    laberintoAux[gx][gy] = 'K';
                    inky.comido = 'o';
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                // GUINDA
                } else if (laberintoAux[gx][gy] == '6') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = inky.comido;
                    laberintoAux[gx][gy] = 'K';
                    inky.comido = '6';
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                // PASILLO 1
                } else if (gx == pasillo1.x && gy == pasillo1.y)  {
                    laberintoAux[pasillo2.x][pasillo2.y] = 'K';
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    inky.coordenadas.x = pasillo2.x;
                    inky.coordenadas.y = pasillo2.y;
                // PASILLO 2    
                } else if (gx == pasillo2.x && gy == pasillo2.y)  {
                    laberintoAux[pasillo1.x][pasillo1.y] = 'K';
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    inky.coordenadas.x = pasillo1.x;
                    inky.coordenadas.y = pasillo1.y;
                // SPAWN DE FANTASMAS (NO DEBE PODER PASAR)
                } else if (gx == ghostSpawn.x && gy == ghostSpawn.y) {
                    inky.vx = 0;
                    inky.vy = -1;
                // ESPACIO LIBRE
                } else if (laberintoAux[gx][gy] == ' ') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = inky.comido;
                    laberintoAux[gx][gy] = 'K';
                    inky.comido = ' ';
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                // COLISION CON PACMAN
                } else if (laberintoAux[gx][gy] == 'C') {
                    if (namnam == 0) {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = inky.comido;
                        laberintoAux[gx][gy] = 'K';
                        inky.comido = ' ';
                        inky.coordenadas.x = gx;
                        inky.coordenadas.y = gy;
                        muertePacman = 1;
                    } else {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = inky.comido;
                        laberintoAux[gx][gy] = 'C';
                        pacmanX.score += 10;
                        initializeGhost(&inky,13,time(NULL),ghostSpawn);
                    }  
                }
            }

            //----CLYDE----//
            //-------------//
            if (clyde.spawned == 1) {
                gx = clyde.coordenadas.x + clyde.vx;
                gy = clyde.coordenadas.y + clyde.vy;

                // PARED
                if (laberintoAux[gx][gy] == '#' || laberintoAux[gx][gy] == 'B' || laberintoAux[gx][gy] == 'M' || laberintoAux[gx][gy] == 'K') {
                    if (clyde.vx != 0) {
                        clyde.vx = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        clyde.vy = arrayGhostMov[rIndex];
                    } else {
                        clyde.vy = 0;
                        srand(time(NULL));
                        int rIndex = rand() % 2;
                        clyde.vx = arrayGhostMov[rIndex];
                    }
                // PUNTO NORMAL       
                } else if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = clyde.comido;
                    laberintoAux[gx][gy] = 'E';
                    clyde.comido = '.';
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = clyde.comido;
                    laberintoAux[gx][gy] = 'E';
                    clyde.comido = 'o';
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                // GUINDA
                } else if (laberintoAux[gx][gy] == '6') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = clyde.comido;
                    laberintoAux[gx][gy] = 'E';
                    clyde.comido = '6';
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                // PASILLO 1
                } else if (gx == pasillo1.x && gy == pasillo1.y)  {
                    laberintoAux[pasillo2.x][pasillo2.y] = 'E';
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    clyde.coordenadas.x = pasillo2.x;
                    clyde.coordenadas.y = pasillo2.y;
                // PASILLO 2    
                } else if (gx == pasillo2.x && gy == pasillo2.y)  {
                    laberintoAux[pasillo1.x][pasillo1.y] = 'E';
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    clyde.coordenadas.x = pasillo1.x;
                    clyde.coordenadas.y = pasillo1.y;
                // SPAWN DE FANTASMAS (NO DEBE PODER PASAR)
                } else if (gx == ghostSpawn.x && gy == ghostSpawn.y) {
                    clyde.vx = 0;
                    clyde.vy = -1;
                // ESPACIO LIBRE
                } else if (laberintoAux[gx][gy] == ' ') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = clyde.comido;
                    laberintoAux[gx][gy] = 'E';
                    clyde.comido = ' ';
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                // COLISION CON PACMAN
                } else if (laberintoAux[gx][gy] == 'C') {
                    if (namnam == 0) {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = clyde.comido;
                        laberintoAux[gx][gy] = 'E';
                        clyde.comido = ' ';
                        clyde.coordenadas.x = gx;
                        clyde.coordenadas.y = gy;
                        muertePacman = 1;
                    } else {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = clyde.comido;
                        laberintoAux[gx][gy] = 'C';
                        pacmanX.score += 10;
                        initializeGhost(&clyde,19,time(NULL),ghostSpawn);
                    }  
                }
            }

            //----SPAWN DE GUINDAS----//
            // Con un temporizador, ir spawneando las guindas.
            currentGameTime = time(NULL);
            tiempoDeGuinda = difftime(currentGameTime,startGuindasTime);
            if (tiempoDeGuinda == guindasTimeSpawn) {
                if (laberintoAux[pacmanSpawn.x][pacmanSpawn.y] == 'C') {
                    segundosParaComer += 2;
                } else if (laberintoAux[pacmanSpawn.x][pacmanSpawn.y] == 'B') {
                    blinky.comido = '6';
                } else if (laberintoAux[pacmanSpawn.x][pacmanSpawn.y] == 'M') {
                    pinky.comido = '6';
                } else if (laberintoAux[pacmanSpawn.x][pacmanSpawn.y] == 'K') {
                    inky.comido = '6';
                } else if (laberintoAux[pacmanSpawn.x][pacmanSpawn.y] == 'E') {
                    clyde.comido = '6';
                } else {
                    laberintoAux[pacmanSpawn.x][pacmanSpawn.y] = '6';
                }
                startGuindasTime = time(NULL);
            }

            //----TIEMPO PARA COMER FANTASMAS----//
            if (namnam == 1) {
                currentGameTime = time(NULL);
                tiempoDeComida = difftime(currentGameTime,timeToEat);
                if (tiempoDeComida >= segundosParaComer) {
                    namnam = 0;
                }
            }
            
            //----TEMPORIZADOR----//
            // Si el temporizador llega a cero, Pacman pierde una vida, y el flag "muertePacman" cambiará a 1, es decir, True.
            currentGameTime = time(NULL);
            tiempoDeJuego = difftime(currentGameTime,startGameTime);
            if (tiempoDeJuego >= 180) {
                muertePacman = 1;
            }

            // IMPRIMIR LABERINTO
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole,GREEN);
            printf("Score: %d   Vidas: %d\n",pacmanX.score,pacmanX.vidas);
            SetConsoleTextAttribute(hConsole,WHITE);
            imprimirLaberinto(filas+2,columnas+2,laberintoAux,namnam);
            SetConsoleTextAttribute(hConsole,GREEN);
            printf("Tiempo de Juego: %.0lf   SPAWN GUINDAS: %.0lf   TIEMPO PARA COMER: %.0lf",tiempoDeJuego,tiempoDeGuinda,tiempoDeComida);
            SetConsoleTextAttribute(hConsole,WHITE);

            Sleep(100);
            setCursorPosition(0,0);

            // Si Pacman pierde todas sus vidas, el flag de "finDelJuego" se vuelve 1, es decir, True.
            if (pacmanX.vidas == 0 || conteoPuntitosAux == 0) {
                finDelJuego = 1;
            }
        }
        system("cls");
        if (pacmanX.vidas == 0) {
            printGameOver();
            Sleep(2000);
        } else {
            printf("GANASTE!");
            Sleep(2000);
        }
        /*
            - Guardar Score en un Array (3 maximos Scores).
            - Generar un archivo "resultados_XX.out" (XX representa el dia de la partida) con las siguientes estadisticas:
                ● La cantidad de puntos pequeños que se comieron.
                ● La cantidad de puntos grandes que se comieron.
                ● La cantidad de veces que PacMan atacó a algún fantasma
                ● La cantidad de muertes de PacMan
                ● Puntaje obtenido
                ● Tiempo Total de juego
            - Las estadisticas anteriores tambien se deben mostrar al final del juego.
        */
        printf("\n\n0. Jugar otra vez\n1. Salir del Juego\nEscoja una opcion: ");
        scanf("%d",&salirDelJuego); 
    }

    system("cls");
    printf("\n\nADIOS,TEN UN BUEN DIA!");
    return 0;
}