#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// ---------------------------------------------------Estructuras-------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

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
    char atravesado;
    //temporizador de spawneo
}ghost;

typedef struct datosDePartida{
    int id;
    int scoreFinal;
    int fantasmasComidos;
    int muertes;
}matchData;

// ----------------------------------------------------Funciones--------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

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
    for (int i=0;i<filas;i++){
        for (int j=0;j<columnas;j++){
            printf("%c",laberint[i][j]);
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

// ------------------------------------------------------Main-----------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

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

    // Conteo de los puntos totales que estan esparcidos en el laberinto
    int conteoPuntitosEnMapa = 0;

    // Delimitar el laberinto
    delimitarLaberinto(filas+2,columnas+2,laberinto);

    // Inicializar laberinto
    // Obtiene los carácters del archivo
    for (int i=1;i<filas+1;i++){
        for (int j=1;j<columnas+1;j++){
            caracter = fgetc(fp);
            if (caracter == 'x') {
                laberinto[i][j] = '.';
                conteoPuntitosEnMapa += 1;
            } else if (caracter == 'X') {
                laberinto[i][j] = 'o';
                conteoPuntitosEnMapa += 1;
            } else if (caracter == 'w') {
                laberinto[i][j] = '#';
            } else if (caracter == 'P') {
                laberinto[i][j] = '*';
            } else if (caracter == 'p') {
                laberinto[i][j] = 'C';
                pacmanSpawn.x = i; // Se guardan las coordenadas del punto de aparición de Pacman
                pacmanSpawn.y = j;
            } else if (caracter == 'S') {
                laberinto[i][j] = 'F';
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

    while (salirDelJuego != 1) {
        /*
        Hacer un "while(salirDelJuego)" desde aqui.
        La opción de salir del juego estará disponible cada vez que Pacman gane o pierda sus 3 vidas.
        Se le preguntará al usuario si desea volver a jugar.
        */

        copiarMatriz(filas+2,columnas+2,laberinto,laberintoAux);
        //imprimirLaberinto(filas+2,columnas+2,laberintoAux);

        // INICIALIZANDO PACMAN
        pacmanX.coordenadas = pacmanSpawn; // Coordenadas iniciales
        pacmanX.score = 0;
        pacmanX.vidas = 3;
        pacmanX.vx = 0;
        pacmanX.vy = 0;

        // INICIALIZANDO FANTASMAS
        blinky.nombre = "Blinky";         // 8
        blinky.coordenadas = ghostSpawn;
        blinky.vx = 0;
        blinky.vy = 0;
        blinky.comible = 0;

        pinky.nombre = "Pinky";           // M
        pinky.coordenadas = ghostSpawn;
        pinky.vx = 0;
        pinky.vy = 0;
        pinky.comible = 0;

        inky.nombre = "Inky";            // K
        inky.coordenadas = ghostSpawn;
        inky.vx = 0;
        inky.vy = 0;
        inky.comible = 0;

        clyde.nombre = "Clyde";          // E
        clyde.coordenadas = ghostSpawn;
        clyde.vx = 0;
        clyde.vy = 0;
        clyde.comible = 0;

        //char ch;
        int finDelJuego = 0;
        int muertePacman = 0;
        int conteoPuntitosAux = conteoPuntitosEnMapa;
        int guindasPorAparecer = guindas;

        /*
            Hacer un "while(finDelJuego)" desde aqui.
            El fin del juego dependera si Pacman gana o pierde sus 3 vidas.
            Si se le acaba el tiempo durante una ronda este perderá una vida.
            Si es comido por un fantasma perderá una vida.
            Luego del fin del juego, se mostrará por pantalla si se ganó o se perdió la partida.
            Luego por pantalla se preguntará si quiere seguir jugando o quiere salir del juego.
        */

        // Limpia la consola de comandos
        system("cls");

        // Mientras el flag de "finDeJuego" sea distinto de 1, es decir, False, el juego sigue. 
        while (finDelJuego != 1) {

            
            // Verifica si Pacman ha muerto para ver si se reinicia el tablero
            if (muertePacman == 1) {
                copiarMatriz(filas+2,columnas+2,laberinto,laberintoAux); // Se reinicia el tablero
                pacmanX.coordenadas = pacmanSpawn; // Se reinicia la posición de Pacman
                pacmanX.score = 0; // Se reinicia el Score de Pacman
                pacmanX.vx = 0; // Se reinicia el movimiento de Pacman
                pacmanX.vy = 0; // Se reinicia el movimiento de Pacman

                blinky.coordenadas = ghostSpawn;
                blinky.vx = 0;
                blinky.vy = 0;
                blinky.comible = 0;
                blinky.atravesado = ' ';
                // Reinicio de temporizador de spawn

                pinky.coordenadas = ghostSpawn;
                pinky.vx = 0;
                pinky.vy = 0;
                pinky.comible = 0;
                pinky.atravesado = ' ';
                // Reinicio de temporizador de spawn

                inky.coordenadas = ghostSpawn;
                inky.vx = 0;
                inky.vy = 0;
                inky.comible = 0;
                inky.atravesado = ' ';
                // Reinicio de temporizador de spawn

                clyde.coordenadas = ghostSpawn;
                clyde.vx = 0;
                clyde.vy = 0;
                clyde.comible = 0;
                clyde.atravesado = ' ';
                // Reinicio de temporizador de spawn
            
                conteoPuntitosAux = conteoPuntitosEnMapa; // Reinicio de la cuenta de puntitos comidos.
                muertePacman = 0;

                // Reinicio de temporizador de juego
                // Reinicio de temporizador de guindas
            }

            // USER INPUT, recibe las teclas que presiona el usuario para controlar a Pacman
            if (_kbhit()) {
                char ch = _getch();
                pacmanX.vx = 0; // Se reinicia el movimiento de Pacman
                pacmanX.vy = 0; // Se reinicia el movimiento de Pacman
                // a = 97; d = 100; w = 119; s = 115
                switch(ch) {
                    case 97:
                        pacmanX.vy = -1;
                        break;
                    case 100:
                        pacmanX.vy = 1;
                        break;
                    case 119:
                        pacmanX.vx = -1;
                        Sleep(50);
                        break;
                    case 115:
                        pacmanX.vx = 1;
                        Sleep(50);
                        break;
                    }
                }
                 

            // MOVIMIENTO DE PACMAN
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
            // PASILLO (PENDIENTE)
            } else if (laberintoAux[lx][ly] == '*') {
                pacmanX.score += 7;
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
            // SPAWN DE FANTASMAS (NO DEBE PODER PASAR)
            } else if (laberintoAux[lx][ly] == 'F') {
                pacmanX.score += 7;
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
            // ESPACIO LIBRE
            } else if (laberintoAux[lx][ly] == ' ') {
                laberintoAux[lx][ly] = 'C';
                laberintoAux[pacmanX.coordenadas.x][pacmanX.coordenadas.y] = ' ';
                pacmanX.coordenadas.x = lx;
                pacmanX.coordenadas.y = ly;
            }

            // COLISION CON FANTASMAS (PENDIENTE)
            /*
                Si Pacman colisiona con un fantasma, este perderá una vida, y el flag "muertePacman" cambiará a 1, es decir, True.
        
            */
            
            // MOVIMIENTO FANTASMAS
            /*
                - Los fantasmas podran bloquearse entre sí, no se podrán atravesar.
                - Los fantasmas no podrán salir del spawn todos al mismo tiempo, lo que conlleva a realizar un temporizador para cada uno para permitir
                que estos vayan spawneando por orden.
                - Si dos fantasmas van a chocar entre si, solo cambiaran de dirección.
                - Los fantasmas si podrán atravesar los puntos y guindas, de alguna forma....XD
                - Colision con Pacman
            */

           // SPAWN DE GUINDAS

           /*   
                Con un temporizador, ir spawneando las guindas.
           */

            // TEMPORIZADOR
            /*
                Si el temporizador llega a cero, Pacman pierde una vida, y el flag "muertePacman" cambiará a 1, es decir, True.
            
            */

            // IMPRIMIR LABERINTO
            printf("Score: %d   Vidas: %d\n",pacmanX.score,pacmanX.vidas);
            imprimirLaberinto(filas+2,columnas+2,laberintoAux);
        
            Sleep(100);
            setCursorPosition(0,0);

            // Si Pacman pierde todas sus vidas, el flag de "finDelJuego" se vuelve 1, es decir, True.
            if (pacmanX.vidas == 0 || conteoPuntitosAux == 0) {
                finDelJuego = 1;
            }
        }
        system("cls");
        printf("Fin del juego XD\n");
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