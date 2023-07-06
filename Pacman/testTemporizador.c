#include <stdio.h>
#include <time.h>

int main() {
    time_t start_time = time(NULL);
    time_t current_time;

    while (1) {
        current_time = time(NULL);

        // Calcula el tiempo transcurrido en segundos
        double elapsed_time = difftime(current_time, start_time);

        // Si han pasado 60 segundos, sale del bucle
        if (elapsed_time >= 60)
            break;

        printf("Hola mundo\n");
    }

    return 0;
}