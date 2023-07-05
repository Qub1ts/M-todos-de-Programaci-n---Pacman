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
    char* nombre;
    cord coordenadas;
    int vx;
    int vy;
    int comible;
    char comido;
    int tempSpawn; // Temporizador de spawneo
    int spawned;
}ghost;

typedef struct datosDePartida{
    int id;
    int scoreFinal;
    int fantasmasComidos;
    int muertes;
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
void imprimirLaberinto(int filas, int columnas,char laberint[filas][columnas]) {
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
                SetConsoleTextAttribute(hConsole,PURPLE);
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

// IMRPIMIR GAMEOVER
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

// ------------------------------------------------------Main----------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------------- //

int main() {

    // ABRIR EL ARCHIVO
    FILE* fp = abrirArchivo("laberinto.txt");

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
    for (int i=1;i<filas+1;i++){
        for (int j=1;j<columnas+1;j++){
            caracter = fgetc(fp); //file get caracter
            if (caracter == 'x') {
                laberinto[i][j] = '.';
                conteoPuntitosEnMapa += 1;
            } else if (caracter == 'X') {
                laberinto[i][j] = 'o';
                conteoPuntitosEnMapa += 1;
            } else if (caracter == 'w') {
                laberinto[i][j] = '#';
            // Pasillo    
            } else if (caracter == 'P') {
                laberinto[i][j] = ' ';
                if (j == 17) {
                    laberinto[i][18] = ' ';
                    pasillo2.x = i;
                    pasillo2.y = 18;
                } 
                if (j == 1) {
                    laberinto[i][0] = ' ';
                    pasillo1.x = i;
                    pasillo1.y = 0;
                }
            } else if (caracter == 'p') {
                laberinto[i][j] = 'C';
                pacmanSpawn.x = i; // Se guardan las coordenadas del punto de aparición de Pacman
                pacmanSpawn.y = j;
            } else if (caracter == 'S') {
                laberinto[i][j] = ' ';
                ghostSpawn.x = i; // Se guardan las coordenadas del punto de aparición de los fantasmas
                ghostSpawn.y = j;
            } else if (caracter == ' ') {
                laberinto[i][j] = ' ';
            }
        }
        caracter = fgetc(fp); // OBTIENE SALTO DE LINEA
    }
    // Cerrar el archivo fuente
    fclose(fp);

    // Imprimir laberinto
    //imprimirLaberinto(filas+2,columnas+2,laberinto);

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
        
    // Hacer un "while(salirDelJuego)" desde aqui.
    // La opción de salir del juego estará disponible cada vez que Pacman gane o pierda sus 3 vidas.
    // Se le preguntará al usuario si desea volver a jugar.
    while (salirDelJuego != 1) {

        copiarMatriz(filas+2,columnas+2,laberinto,laberintoAux);

        //--------------------------------------------INICIALIZANDO PACMAN----------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//
        pacmanX.coordenadas = pacmanSpawn; // Coordenadas iniciales
        pacmanX.score = 0;
        pacmanX.vidas = 3;
        pacmanX.vx = 0;
        pacmanX.vy = 0;

        //-------------------------------------------INICIALIZANDO FANTASMAS--------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//
        blinky.nombre = "Blinky";         // B
        blinky.coordenadas = ghostSpawn;
        blinky.vx = -1;
        blinky.vy = 0;
        blinky.comible = 0;
        blinky.tempSpawn = 1;
        blinky.spawned = 0;

        pinky.nombre = "Pinky";           // M
        pinky.coordenadas = ghostSpawn;
        pinky.vx = -1;
        pinky.vy = 0;
        pinky.comible = 0;
        pinky.tempSpawn = 7;
        pinky.spawned = 0;

        inky.nombre = "Inky";            // K
        inky.coordenadas = ghostSpawn;
        inky.vx = -1;
        inky.vy = 0;
        inky.comible = 0;
        inky.tempSpawn = 13;
        inky.spawned = 0;

        clyde.nombre = "Clyde";          // E
        clyde.coordenadas = ghostSpawn;
        clyde.vx = -1;
        clyde.vy = 0;
        clyde.comible = 0;
        clyde.tempSpawn = 19;
        clyde.spawned = 0;

        int finDelJuego = 0;
        int muertePacman = 0;
        int conteoPuntitosAux = conteoPuntitosEnMapa;
        int guindasPorAparecer = guindas;

        // Limpia la consola de comandos
        system("cls");

        //-----------------------------------------------------FIN DEL JUEGO--------------------------------------------------------------//
        //--------------------------------------------------------------------------------------------------------------------------------//

        // Mientras el flag de "finDeJuego" sea distinto de 1, es decir, False, el juego sigue. 
        while (finDelJuego != 1) {

            // Verifica si Pacman ha muerto para ver si se reinicia el tablero
            if (muertePacman == 1) {
                copiarMatriz(filas+2,columnas+2,laberinto,laberintoAux); // Se reinicia el tablero
                pacmanX.vidas -= 1; // Se le resta una vida a Pacman ya que murió
                pacmanX.coordenadas = pacmanSpawn; // Se reinicia la posición de Pacman
                pacmanX.score = 0; // Se reinicia el Score de Pacman
                pacmanX.vx = 0; // Se reinicia el movimiento de Pacman
                pacmanX.vy = 0; // Se reinicia el movimiento de Pacman

                blinky.coordenadas = ghostSpawn;
                blinky.vx = -1;
                blinky.vy = 0;
                blinky.comible = 0;
                blinky.comido = ' ';
                blinky.tempSpawn = 1; // Reinicio de temporizador de spawn
                blinky.spawned = 0;

                pinky.coordenadas = ghostSpawn;
                pinky.vx = -1;
                pinky.vy = 0;
                pinky.comible = 0;
                pinky.comido = ' ';
                pinky.tempSpawn = 7; // Reinicio de temporizador de spawn
                pinky.spawned = 0;

                inky.coordenadas = ghostSpawn;
                inky.vx = -1;
                inky.vy = 0;
                inky.comible = 0;
                inky.comido = ' ';
                inky.tempSpawn = 13; // Reinicio de temporizador de spawn
                inky.spawned = 0;

                clyde.coordenadas = ghostSpawn;
                clyde.vx = -1;
                clyde.vy = 0;
                clyde.comible = 0;
                clyde.comido = ' ';
                clyde.tempSpawn = 19; // Reinicio de temporizador de spawn
                clyde.spawned = 0;
            
                conteoPuntitosAux = conteoPuntitosEnMapa; // Reinicio de la cuenta de puntitos comidos.
                muertePacman = 0;

                // PARPADEO DE TABLERO
                if (pacmanX.vidas != 0) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole,GREEN);printf("Score: %d   Vidas: %d\n",pacmanX.score,pacmanX.vidas);SetConsoleTextAttribute(hConsole,WHITE);
                    imprimirLaberinto(filas+2,columnas+2,laberintoAux);
                    Sleep(300);system("cls");Sleep(300);
                    SetConsoleTextAttribute(hConsole,GREEN);printf("Score: %d   Vidas: %d\n",pacmanX.score,pacmanX.vidas);SetConsoleTextAttribute(hConsole,WHITE);
                    imprimirLaberinto(filas+2,columnas+2,laberintoAux);
                    Sleep(300);system("cls");Sleep(300);
                    SetConsoleTextAttribute(hConsole,GREEN);printf("Score: %d   Vidas: %d\n",pacmanX.score,pacmanX.vidas);SetConsoleTextAttribute(hConsole,WHITE);
                    imprimirLaberinto(filas+2,columnas+2,laberintoAux);
                    Sleep(300);system("cls");Sleep(300);
                }
                
                // Reinicio de temporizador de juego
                // Reinicio de temporizador de guindas
            }

            //--------------------------------------------------------USER INPUT--------------------------------------------------------------//
            //--------------------------------------------------------------------------------------------------------------------------------//
        
            // Recibe las teclas que presiona el usuario para controlar a Pacman
            if (_kbhit()) {
                char ch = _getch();
                pacmanX.vx = 0; // Se reinicia el movimiento de Pacman
                pacmanX.vy = 0; // Se reinicia el movimiento de Pacman
                // a = 97; d = 100; w = 119; s = 115
                switch(ch) {
                    case 97: // A
                        pacmanX.vy = -1;
                        break;
                    case 100: // D
                        pacmanX.vy = 1;
                        break;
                    case 119: // W
                        pacmanX.vx = -1;
                        Sleep(50);
                        break;
                    case 115: // S
                        pacmanX.vx = 1;
                        Sleep(50);
                        break;
                    }
                }
                
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
            // COLISION CON FANTASMAS (PENDIENTE)
            } else if (laberintoAux[lx][ly] == 'B' || laberintoAux[lx][ly] == 'M' || laberintoAux[lx][ly] == 'K' || laberintoAux[lx][ly] == 'E') {
                laberintoAux[lx][ly] = 'B';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
                muertePacman = 1;
            }

            //-----------------------------------------------------SPAWN FANTASMAS------------------------------------------------------------//
            //--------------------------------------------------------------------------------------------------------------------------------//
            if (blinky.spawned == 0) {
                blinky.tempSpawn -= 1;
                if (blinky.tempSpawn == 0) {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = 'B';
                    blinky.spawned = 1;
                }
            }

            if (pinky.spawned == 0) {
                pinky.tempSpawn -= 1;
                if (pinky.tempSpawn == 0) {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = 'M';
                    pinky.spawned = 1;
                }
            }

            if (inky.spawned == 0) {
                inky.tempSpawn -= 1;
                if (inky.tempSpawn == 0) {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = 'K';
                    inky.spawned = 1;
                }
            }

            if (clyde.spawned == 0) {
                clyde.tempSpawn -= 1;
                if (clyde.tempSpawn == 0) {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = 'E';
                    clyde.spawned = 1;
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
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
            // AÑADIR MAS MOVIMIENTOS RANDOM
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
                    gx = blinky.coordenadas.x + blinky.vx;
                    gy = blinky.coordenadas.y + blinky.vy;
                }
                // PUNTO NORMAL 
                if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'B';
                    if (blinky.comido == ' ') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    } else if (blinky.comido == '.') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = '.';
                    } else if (blinky.comido == 'o') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = 'o';
                    }
                    blinky.comido = '.';
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'B';
                    if (blinky.comido == ' ') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    } else if (blinky.comido == '.') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = '.';
                    } else if (blinky.comido == 'o') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = 'o';
                    }
                    blinky.comido = 'o';
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
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'B';
                    if (blinky.comido == ' ') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                        blinky.comido = ' '; 
                    } else if (blinky.comido == '.') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = '.';
                        blinky.comido = ' '; 
                    } else if (blinky.comido == 'o') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = 'o';
                        blinky.comido = ' '; 
                    }
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                } else if (laberintoAux[gx][gy] == 'C') {
                    laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'B';
                    if (blinky.comido == ' ') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = ' ';
                        blinky.comido = ' '; 
                    } else if (blinky.comido == '.') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = '.';
                        blinky.comido = ' '; 
                    } else if (blinky.comido == 'o') {
                        laberintoAux[blinky.coordenadas.x][blinky.coordenadas.y] = 'o';
                        blinky.comido = ' '; 
                    }
                    blinky.coordenadas.x = gx;
                    blinky.coordenadas.y = gy;
                    muertePacman = 1;
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
                    gx = pinky.coordenadas.x + pinky.vx;
                    gy = pinky.coordenadas.y + pinky.vy;
                }
                // PUNTO NORMAL 
                if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'M';
                    if (pinky.comido == ' ') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    } else if (pinky.comido == '.') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = '.';
                    } else if (pinky.comido == 'o') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = 'o';
                    }
                    pinky.comido = '.';
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'M';
                    if (pinky.comido == ' ') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    } else if (pinky.comido == '.') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = '.';
                    } else if (pinky.comido == 'o') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = 'o';
                    }
                    pinky.comido = 'o';
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                // PASILLO 1
                } else if (gx == pasillo1.x && gy == pasillo1.y)  {
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
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'M';
                    if (pinky.comido == ' ') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                        pinky.comido = ' '; 
                    } else if (pinky.comido == '.') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = '.';
                        pinky.comido = ' '; 
                    } else if (pinky.comido == 'o') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = 'o';
                        pinky.comido = ' '; 
                    }
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                } else if (laberintoAux[gx][gy] == 'C') {
                    laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'M';
                    if (pinky.comido == ' ') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = ' ';
                        pinky.comido = ' '; 
                    } else if (pinky.comido == '.') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = '.';
                        pinky.comido = ' '; 
                    } else if (pinky.comido == 'o') {
                        laberintoAux[pinky.coordenadas.x][pinky.coordenadas.y] = 'o';
                        pinky.comido = ' '; 
                    }
                    pinky.coordenadas.x = gx;
                    pinky.coordenadas.y = gy;
                    muertePacman = 1;
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
                    gx = inky.coordenadas.x + inky.vx;
                    gy = inky.coordenadas.y + inky.vy;
                }
                // PUNTO NORMAL 
                if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'K';
                    if (inky.comido == ' ') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    } else if (inky.comido == '.') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = '.';
                    } else if (inky.comido == 'o') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = 'o';
                    }
                    inky.comido = '.';
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'K';
                    if (inky.comido == ' ') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    } else if (inky.comido == '.') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = '.';
                    } else if (inky.comido == 'o') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = 'o';
                    }
                    inky.comido = 'o';
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
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'K';
                    if (inky.comido == ' ') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                        inky.comido = ' '; 
                    } else if (inky.comido == '.') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = '.';
                        inky.comido = ' '; 
                    } else if (inky.comido == 'o') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = 'o';
                        inky.comido = ' '; 
                    }
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                } else if (laberintoAux[gx][gy] == 'C') {
                    laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'K';
                    if (inky.comido == ' ') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = ' ';
                        inky.comido = ' '; 
                    } else if (inky.comido == '.') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = '.';
                        inky.comido = ' '; 
                    } else if (inky.comido == 'o') {
                        laberintoAux[inky.coordenadas.x][inky.coordenadas.y] = 'o';
                        inky.comido = ' '; 
                    }
                    inky.coordenadas.x = gx;
                    inky.coordenadas.y = gy;
                    muertePacman = 1;
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
                    gx = clyde.coordenadas.x + clyde.vx;
                    gy = clyde.coordenadas.y + clyde.vy;
                }
                // PUNTO NORMAL 
                if (laberintoAux[gx][gy] == '.') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'E';
                    if (clyde.comido == ' ') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    } else if (clyde.comido == '.') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = '.';
                    } else if (clyde.comido == 'o') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = 'o';
                    }
                    clyde.comido = '.';
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                // PUNTO GRANDE 
                } else if (laberintoAux[gx][gy] == 'o') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'E';
                    if (clyde.comido == ' ') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    } else if (clyde.comido == '.') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = '.';
                    } else if (clyde.comido == 'o') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = 'o';
                    }
                    clyde.comido = 'o';
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
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'E';
                    if (clyde.comido == ' ') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                        clyde.comido = ' '; 
                    } else if (clyde.comido == '.') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = '.';
                        clyde.comido = ' '; 
                    } else if (clyde.comido == 'o') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = 'o';
                        clyde.comido = ' '; 
                    }
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                } else if (laberintoAux[gx][gy] == 'C') {
                    laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                    laberintoAux[gx][gy] = 'E';
                    if (clyde.comido == ' ') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = ' ';
                        clyde.comido = ' '; 
                    } else if (clyde.comido == '.') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = '.';
                        clyde.comido = ' '; 
                    } else if (clyde.comido == 'o') {
                        laberintoAux[clyde.coordenadas.x][clyde.coordenadas.y] = 'o';
                        clyde.comido = ' '; 
                    }
                    clyde.coordenadas.x = gx;
                    clyde.coordenadas.y = gy;
                    muertePacman = 1;
                }
            }

            // SPAWN DE GUINDAS
                // Con un temporizador, ir spawneando las guindas.
            

            // TEMPORIZADOR
                // Si el temporizador llega a cero, Pacman pierde una vida, y el flag "muertePacman" cambiará a 1, es decir, True.
            

            // IMPRIMIR LABERINTO
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole,GREEN);
            printf("Score: %d   Vidas: %d\n",pacmanX.score,pacmanX.vidas);
            SetConsoleTextAttribute(hConsole,WHITE);
            imprimirLaberinto(filas+2,columnas+2,laberintoAux);
        
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
            system("cls");
        } else {
            printf("GANASTE!");
            Sleep(2000);
            system("cls");
        }
        printf("0. Jugar otra vez\n1. Salir del Juego\nEscoja una opcion: ");
        scanf("%d",&salirDelJuego);
        /*
            - Mostrar resultado final del juego (Gano o perdió).
                - Si conteoPuntitosAux == 0, ganó, si no perdió.
            - Guardar Score en un Array (3 maximos Scores).
            - Generar un archivo "resultados_XX.out" (XX representa el dia de la partida) con las sgtes estadisticas:
                ● La cantidad de puntos pequeños que se comieron.
                ● La cantidad de puntos grandes que se comieron.
                ● La cantidad de veces que PacMan atacó a algún fantasma
                ● La cantidad de muertes de PacMan
                ● Puntaje obtenido
                ● Tiempo Total de juego
            - Las estadisticas anteriores tambien se deben mostrar al final del juego.
            - Preguntar al usuario si quiere seguir jugando.
        */
    }

    system("cls");
    printf("\n\nCHAO XD");
    return 0;
}