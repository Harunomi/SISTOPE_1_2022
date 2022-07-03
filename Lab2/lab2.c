#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "Funciones.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

FILE *archivoEntrada;
pthread_mutex_t mLectura;
pthread_mutex_t mEscritura;
entradaComando entrada;
resultado r;
int nHebras;
int nVis;
int *rangoDiscos;

int key = 0; 

void *ejecucion(void *unused){
    if (!feof(archivoEntrada)){
        pthread_mutex_lock(&mLectura);

        if (key == 0){
            // leemos una visibilidad del archivo
            visibilidades actual;
            if(fscanf(archivoEntrada,"%f,%f,%f,%f,%f", &actual.ejeU,&actual.ejeV,actual.&actual.valorImaginario,&actual.ruido)){

                // ubicamos en que disco se encuentra la visibilidad actual
                double distancia = sqrt(pow(actual.valorReal,2) + pow(actual.valorImaginario,2));

            }
        }
    }
}

int main(int argc, char *argv[]){
    int opt;


    while ((opt = getopt(argc,argv, "i:o:d:n:h:c:b"))!= -1){
        switch (opt){
            case 'i':
                strcpy(entrada.archivoEntrada,optarg);
                break;
            case 'o':
                strcpy(entrada.archivoSalida,optarg);
                break;
            case 'd':
                entrada.anchoDisco = atoi(optarg);
                break;
            case 'n':
                entrada.cantDiscos = atoi(optarg);
                break;
            case 'h':
                entrada.cantHebras = atoi(optarg);
                break;
            case 'c':
                entrada.chunk = atoi(optarg);
                break;
            case 'b':
                entrada.flag = 1;
                break;
        }
    }

    verificador_entradas(entrada);
    rangoDiscos = (int*)malloc(sizeof(int)*entrada.cantDiscos); 
    for (int i = 0; i < entrada.cantDiscos; i++) {
        rangoDiscos[i] = entrada.anchoDisco*i;
    }

    pthread_mutex_init(&mLectura, NULL);
    pthread_mutex_init(&mEscritura, NULL);  

    pthread_attr_t attr[entrada.cantHebras];

    for (int i = 0; i < entrada.cantHebras; i++) {
        pthread_attr_init(&attr[i]);
    }

    // creamos las hebras

    pthread_t thread[entrada.cantHebras];

    for (int i = 0; i < entrada.cantHebras; i++) {
        pthread_create(&thread[i], &attr[i],ejecucion,NULL);
        
    }

    fclose(archivoEntrada);

    // dps de esto escribimos el archivo de salida
    // y printeamos por pantalla dependiendo de la entrada.flag





    // probablewmente para ahorrarme trabajo, al printear los rresultados
    // por pantalla, simplemente lea el archivo que ya acabo de escribir 


    


}