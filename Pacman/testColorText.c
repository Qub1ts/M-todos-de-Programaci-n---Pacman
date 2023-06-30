#include <stdio.h>
#include <windows.h>

// Definición de colores
#define BLACK 0
#define RED 12
#define GREEN 10
#define YELLOW 14
#define BLUE 9
#define MAGENTA 13
#define CYAN 11
#define WHITE 15

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Establecer el color de fondo y el color del texto
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    printf("Texto en color");
    printf("\n");

    // Restablecer el color a los valores predeterminados
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return 0;
}