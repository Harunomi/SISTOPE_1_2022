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
resultado  *discos;
int nHebras;
int nVis;
int *rangoDiscos;

int key = 0; 

void *ejecucion(void *unused){
    if (!feof(archivoEntrada)){
        pthread_mutex_lock(&mLectura);
        int contador = 0;
        while (contador < entrada.chunk && key != 1 && !feof(archivoEntrada)){
            if (key == 0){
                // leemos una visibilidad del archivo
                visibilidades actual;
                if(fscanf(archivoEntrada,"%f,%f,%f,%f,%f", &actual.ejeU,&actual.ejeV,&actual.valorReal,&actual.valorImaginario,&actual.ruido)){

                    // ubicamos en que disco se encuentra la visibilidad actual
                    double distancia = sqrt(pow(actual.ejeU,2) + pow(actual.ejeV,2));
                    int pos;

                    for (int i = 0; i < entrada.cantDiscos ; i++) {
                        if(distancia >= rangoDiscos[i] && distancia < rangoDiscos[i+1] && i+1 < entrada.cantDiscos ){
                            pos = i;
                        }
                        if (distancia >= rangoDiscos[entrada.cantDiscos-1] && i == entrada.cantDiscos-1){
                            pos = i;
                        }
                    }
                    
                    contador++;
                    pthread_mutex_lock(&mEscritura);
                    // guardamos las visibilidades leidas 
                    discos[pos].contador++;
                    discos[pos].mediaReal = discos[pos].mediaReal + actual.valorReal;
                    discos[pos].mediaImaginaria = discos[pos].mediaImaginaria + actual.valorImaginario;
                    discos[pos].ruidoTotal = discos[pos].ruidoTotal + actual.ruido;
                    discos[pos].potencia = discos[pos].potencia + (sqrt(pow(actual.valorReal,2) + pow(actual.valorImaginario,2)));
                    
                    pthread_mutex_unlock(&mEscritura);
                }
            }else{
                key = 0;
            }
        }
        pthread_mutex_unlock(&mLectura);
        ejecucion(NULL);
    }else{
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
    
}

int main(int argc, char *argv[]){
    int opt;
    FILE *archivoSalida;
    char fila[1000];


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

    discos = (resultado*)malloc(sizeof(resultado)*entrada.cantDiscos);
    for (int i = 0; i < entrada.cantDiscos; i++) {
        discos[i].mediaReal = 0;
        discos[i].mediaImaginaria = 0;
        discos[i].potencia = 0;
        discos[i].ruidoTotal = 0;
    }
    archivoEntrada = fopen(entrada.archivoEntrada,"r");

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

    archivoSalida = fopen(entrada.archivoSalida,"w");
    

    // dps de esto escribimos el archivo de salida
    // y printeamos por pantalla dependiendo de la entrada.flag
    for (int i = 0; i < entrada.cantDiscos; i++) {
        discos[i].potencia = sqrt(discos[i].potencia);
        discos[i].mediaReal = discos[i].mediaReal / discos[i].contador;
        discos[i].mediaImaginaria = discos[i].mediaImaginaria / discos[i].contador;
        fprintf(archivoSalida,"Disco %d:\n",i+1);
        fprintf(archivoSalida,"Media real: %Lf\n",discos[i].mediaReal);
        fprintf(archivoSalida,"Media imaginaria: %Lf\n",discos[i].mediaImaginaria);
        fprintf(archivoSalida,"Potencia: %Lf\n",discos[i].potencia);
        fprintf(archivoSalida,"Ruido Total: %Lf\n",discos[i].ruidoTotal);
    }




    // probablewmente para ahorrarme trabajo, al printear los rresultados
    // por pantalla, simplemente lea el archivo que ya acabo de escribir 


    
    fclose(archivoSalida);
    free(rangoDiscos);
    free(discos);
    pthread_mutex_destroy(&mLectura);
    pthread_mutex_destroy(&mEscritura);

    printearResultados(entrada.archivoSalida);

    

    return 0;
}