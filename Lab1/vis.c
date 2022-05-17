#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "Funciones.h"


int main(int argc, char **argv){
    int totalV = atoi(argv[0]); // el total de visibilidades lo recibimos como string, entonces lo pasamos a entero
    resultado r;
    visibilidades *v = (visibilidades*)malloc(sizeof(visibilidades)*totalV);
    visibilidades *v2 = (visibilidades*)malloc(sizeof(visibilidades)*totalV);
    for (int i = 0; i < totalV; i++) {
        // leemos el stdin con cada visibilidad
        read(STDIN_FILENO,&v[i],sizeof(visibilidades));
        v2[i] = v[i]; // se  guarda dicha visibilidad en v2 para posteriormente ser trabajada
    }
    r = calcular(v2,totalV); // hacemos el llamado a la funcion para calcular
    write(STDOUT_FILENO, &r, sizeof(resultado)); // devolvemos dicho resultado al proceso padre
    return 0;
}