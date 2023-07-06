#include <stdio.h>
#include <windows.h>

// Definición de colores
#define BLUE 1
#define RED 4
#define GREEN 10
#define YELLOW 6
#define PURPLE 13
#define WHITE 15


int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,BLUE);
    printf("HOLA XD\n");
    SetConsoleTextAttribute(hConsole,RED);
    printf("HOLA XD\n");
    SetConsoleTextAttribute(hConsole,GREEN);
    printf("HOLA XD\n");
    SetConsoleTextAttribute(hConsole,YELLOW);
    printf("HOLA XD\n");
    SetConsoleTextAttribute(hConsole,PURPLE);
    printf("HOLA XD\n");
    SetConsoleTextAttribute(hConsole,WHITE);
    printf("HOLA XD\n");
    Beep(300,900);

    

    return 0;
}