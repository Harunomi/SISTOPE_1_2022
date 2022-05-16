#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "Funciones.h"

int main(int argc, char **argv){
    int totalV = atoi(argv[0]);
    resultado r;
    visibilidades *v = (visibilidades*)malloc(sizeof(visibilidades)*totalV);
    float mediaReal = 0;
    float mediaImaginaria = 0;
    float potencia = 0;
    float ruidoTotal = 0;
    read(STDIN_FILENO,&v, sizeof(visibilidades*));

    //media reaL
    for (int i = 0; i < totalV-1; i++) {
        printf("%f %f %f %f %f",v[i].ejeU,v[i].ejeV,v[i].valorReal,v[i].valorImaginario,v[i].ruido);
        mediaReal = mediaReal + v[i].valorReal;
        mediaImaginaria = mediaImaginaria + v[i].valorImaginario;
        potencia = potencia + sqrt(pow(v[i].valorReal,2) + pow(v[i].valorImaginario,2));
        ruidoTotal = ruidoTotal + v[i].ruido;
    }
    mediaReal = mediaReal/totalV;
    mediaImaginaria = mediaImaginaria/totalV;

    r.mediaReal = mediaReal;
    r.mediaImaginaria = mediaImaginaria;
    r.potencia = potencia;
    r.ruidoTotal = ruidoTotal;


    write(STDOUT_FILENO, &r, sizeof(resultado));


    
    
    return 0;
}