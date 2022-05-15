/* C Program to count the Number of Lines in a Text File */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "Funciones.h"

#define LECTURA 0
#define ESCRITURA 1


int main(int argc, char *argv[]){   
    entradaComando o;
    visibilidades v;
    o.ptrcsl = 0;
    int opt;
    pid_t pid;
    int status;
    float distancia;
    char *cNum;
    int sizeCNum;
    while((opt = getopt(argc,argv,":i:o:d:n:b")) != -1){
        switch (opt){
            case 'i': 
                strcpy(o.archivoVisibilidades,optarg);
                break;
            case 'o':
                strcpy(o.archivoSalida,optarg);
                break;
            case 'd':
                o.cantDiscos = atoi(optarg);
                break;
            case 'n':
                o.anchoDisco = atoi(optarg);
                break;
            case 'b':
                o.ptrcsl = 1;
                break;
        }
    }
    verificador_entradas(o);
    FILE *fp = fopen(o.archivoVisibilidades,"r");
    // creo un arreglo con los rangos del disco
    int *rangoDiscos = (int*)malloc(sizeof(int)*o.cantDiscos); 
    for (int i = 0; i < o.cantDiscos; i++) {
        rangoDiscos[i] = o.anchoDisco*i;
    }

    // creo un arreglo para guardar el total de visibilidades que le toca a cada disco.
    int *cantidadVisibilidades = (int*)malloc(sizeof(int)*o.cantDiscos);
    //lo inicializo en cero
    for (int i = 0; i < o.cantDiscos; i++) {
        cantidadVisibilidades[i] = 0;
    }

    // creo los pipes de lectura y escritura
    int **arregloPipesEscritura = (int**)malloc(sizeof(int*)*o.cantDiscos);
    int **arregloPipesLectura = (int**)malloc(sizeof(int*)*o.cantDiscos);
    // les asigno memoria a los canales
    for (int i = 0; i < o.cantDiscos; i++) {
        arregloPipesEscritura[i] = (int*)malloc(sizeof(int)*2);
        arregloPipesLectura[i] = (int*)malloc(sizeof(int)*2);
        pipe(arregloPipesEscritura[i]);
        pipe(arregloPipesLectura[i]);
    }   
    // se calcula el total del lineas que tiene el archivo de lectura.
    int totalLineas = cuenta_lineas(o.archivoVisibilidades);

    for (int i = 0; i < o.cantDiscos; i++) {
        pid = fork();

        if (pid != 0){ // soy el padre
            for (int j = 0; j < totalLineas ; j++) {
                // leo la linea del codigo
                fscanf("%f,%f,%f,%f,%f",&v.ejeU,&v.ejeV,&v.valorReal,&v.valorImaginario,&v.ruido);
                // calculo su distancia
                distancia = sqrt(pow(v.ejeU,2) + pow(v.ejeV,2));
                // determino si esa distancia corresponde al disco (proceso) actual
                if(distancia>=rangoDiscos[i] && distancia < rangoDiscos[i+1] && i+1 < o.cantDiscos){   
                    // si corresponde al proceso actual, entonces lo mando por el pipe
                    write(arregloPipesLectura[i][ESCRITURA], v.ejeU, sizeof(float));
                    write(arregloPipesLectura[i][ESCRITURA], v.ejeV, sizeof(float));
                    write(arregloPipesLectura[i][ESCRITURA], v.valorReal, sizeof(float));
                    write(arregloPipesLectura[i][ESCRITURA], v.valorImaginario, sizeof(float));
                    write(arregloPipesLectura[i][ESCRITURA], v.ruido, sizeof(float));
                    cantidadVisibilidades[i] = cantidadVisibilidades[i] + 1;   
                }
                if(distancia >=rangoDiscos[o.cantDiscos-1]){
                    write(arregloPipesLectura[o.cantDiscos-1][ESCRITURA], v.ejeU, sizeof(float));
                    write(arregloPipesLectura[o.cantDiscos-1][ESCRITURA], v.ejeV, sizeof(float));
                    write(arregloPipesLectura[o.cantDiscos-1][ESCRITURA], v.valorReal, sizeof(float));
                    write(arregloPipesLectura[o.cantDiscos-1][ESCRITURA], v.valorImaginario, sizeof(float));
                    write(arregloPipesLectura[o.cantDiscos-1][ESCRITURA], v.ruido, sizeof(float));   
                    cantidadVisibilidades[o.cantDiscos-1] = cantidadVisibilidades[o.cantDiscos-1] + 1;
                }

                waitpid(pid,&status,0);

                close(arregloPipesEscritura[i][ESCRITURA]);

                
            }
        }else{ // soy el hijo
            close(arregloPipesEscritura[i][LECTURA]);
            dup2(arregloPipesEscritura[i][ESCRITURA],STDOUT_FILENO);
            close(arregloPipesEscritura[i][ESCRITURA]);

            close(arregloPipesLectura[i][ESCRITURA]);
            dup2(arregloPipesLectura[i][LECTURA],STDIN_FILENO);
            close(arregloPipesLectura[i][LECTURA]);

            // transformo el total de visibilidades para el respectivo disco a string
            sizeCNum = snprintf(NULL, 0, "%d",cantidadVisibilidades[i]);
            cNum = malloc(sizeCNum + 1);
            snprintf(cNum,sizeCNum+1,"%d",cantidadVisibilidades[i]);

            execl("vis",cNum,NULL);
        }
    }

    
    
    
    
    

	
    
    
	return 0;
}