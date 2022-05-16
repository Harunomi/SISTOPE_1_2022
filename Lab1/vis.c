#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
//#include "Funciones.h"
#include "FuncionesTest.h"

int main(int argc, char **argv){
    int totalV = atoi(argv[0]);
    resultado r;
    visibilidades *v = (visibilidades*)malloc(sizeof(visibilidades)*totalV);
    //visibilidades *retorno = (visibilidades*)malloc(sizeof(visibilidades)*totalV);
    read(STDIN_FILENO,&v, sizeof(visibilidades*));
    r = calcular(v,totalV);
    write(STDOUT_FILENO, &r, sizeof(resultado));
    //retorno = v;
    
    //media reaL
    /*
    for (int t = 0; t < totalV; t++) {
        mediaReal = mediaReal + v[t].valorReal;
        mediaImaginaria = mediaImaginaria + v[t].valorImaginario;
        potencia = potencia + sqrt(pow(v[t].valorReal,2) + pow(v[t].valorImaginario,2));
        ruidoTotal = ruidoTotal + v[t].ruido;
    }
    mediaReal = mediaReal/totalV;
    mediaImaginaria = mediaImaginaria/totalV;
    */


    /*
    write(STDOUT_FILENO, &mediaReal, sizeof(float));
    write(STDOUT_FILENO, &mediaImaginaria, sizeof(float));
    write(STDOUT_FILENO, &potencia, sizeof(float));
    write(STDOUT_FILENO, &ruidoTotal, sizeof(float));
    */
    /*
    gcvt(mediaReal,10,cmr);
    gcvt(mediaImaginaria,10,cmi);
    gcvt(potencia,10,cp);
    gcvt(ruidoTotal,10,cr);
    */
    /*
    r.mediaReal = mediaReal;
    r.mediaImaginaria = mediaImaginaria;
    r.potencia = potencia;
    r.ruidoTotal = ruidoTotal;
    */

    


    
    
    return 0;
}