#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct coord{
    int x;
    int y;
}cord;

typedef struct fantasma{
    char* nombre;
    cord coordenadas;
    int vx;
    int vy;
    int comible;
    int spawned;
    char comido;
    int spawnSpaces;
    time_t spawnTimer;
}ghost;

void initializeGhost(ghost* ghost,int spawnSpaces,time_t spawnTimer,cord ghostSpawn) {
    ghost->spawned = 0;
    ghost->comible = 0;
    ghost->spawnTimer = spawnTimer;
    ghost->coordenadas = ghostSpawn; 
    ghost->comido = ' ';
    ghost->vx = -1;
    ghost->vy = 0;
    ghost->spawnSpaces = spawnSpaces;
}

int main() {
    cord ghostSpawn;
    ghostSpawn.x = 3;
    ghostSpawn.y = 4;
    ghost xd;
    xd.nombre = "old";
    initializeGhost(&xd,7,10,ghostSpawn);
    printf("%d",xd.spawned);

}