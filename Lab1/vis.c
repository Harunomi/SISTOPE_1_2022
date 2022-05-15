#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "Funciones.h"

int main(int argc, char **argv){
    totalV = atoi(argv[1]);
    visibilidades *v = (visibilidades*)malloc(sizeof(visibilidades)*totalV);

    for (int i = 0; i < totalV; i++) {
        read(STDIN_FILENO,v[i].ejeU, sizeof(float));
        read(STDIN_FILENO,v[i].ejeV, sizeof(float));
        read(STDIN_FILENO,v[i].valorReal, sizeof(float));
        read(STDIN_FILENO,v[i].valorImaginario, sizeof(float));
        read(STDIN_FILENO,v[i].ruido, sizeof(float));
        printf("%f %f %f %f %f\n",v[i].ejeU,v[i].ejeV,v[i].valorReal,v.valorImaginario,v.ruido);
    }
    
}