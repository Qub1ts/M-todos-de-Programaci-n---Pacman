#include <stdio.h>
#include <windows.h>

// Definición de colores


int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Imprimir palabra en rojo
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("Rojo ");

    // Imprimir palabra en verde
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("Verde ");

    // Imprimir palabra en azul
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    printf("Azul\n");

    // Restablecer el color a los valores predeterminados
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return 0;
}