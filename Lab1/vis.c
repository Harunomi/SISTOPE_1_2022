#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "Funciones.h"

int main(int argc, char **argv){
    int totalV = atoi(argv[0]);
    visibilidades *v = (visibilidades*)malloc(sizeof(visibilidades)*totalV);

    for (int i = 0; i < totalV; i++) {
        read(STDIN_FILENO,&v[i], sizeof(visibilidades));

        write(STDOUT_FILENO,&v[i], sizeof(visibilidades));
        
    }
    return 0;
}