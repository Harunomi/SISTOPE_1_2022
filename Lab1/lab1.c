#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "Funciones.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[]){   
    entradaComando o;
    visibilidades v;
    visibilidades *out;
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
    FILE * fp;
    FILE * salida = fopen(o.archivoSalida,"w");
    // creo un arreglo con los rangos del disco
    int *rangoDiscos = (int*)malloc(sizeof(int)*o.cantDiscos); 
    for (int i = 0; i < o.cantDiscos; i++) {
        rangoDiscos[i] = o.anchoDisco*i;
    }

    // creo un arreglo para guardar el total de visibilidades que le toca a cada disco.
    int *cantidadVisibilidades = (int*)malloc(sizeof(int)*o.cantDiscos);
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

    cantidadVisibilidades = totalVisibilidades(o.archivoVisibilidades,totalLineas,o.cantDiscos,rangoDiscos);
    int aux;
    resultado r;
    int i;
    for (i = 0; i < o.cantDiscos; i++) {
        pid = fork();
        
        if (pid != 0){ // soy el padre
            close(arregloPipesLectura[i][LECTURA]); // se cierra este pipe puesto que sera usado para enviar datos 
            fp = fopen(o.archivoVisibilidades,"r");
            aux = 0;
            for (int j = 0; j < totalLineas ; j++) {
                // leo la linea del codigo
                fscanf(fp,"%f,%f,%f,%f,%f",&v.ejeU,&v.ejeV,&v.valorReal,&v.valorImaginario,&v.ruido);
                // calculo su distancia
                distancia = sqrt(pow(v.ejeU,2) + pow(v.ejeV,2));
                // determino si esa distancia corresponde al disco (proceso) actual             
                if(distancia>=rangoDiscos[i] && distancia < rangoDiscos[i+1] && i+1 < o.cantDiscos){   
                    // si corresponde al proceso actual, entonces lo mando por el pipe 
                    write(arregloPipesLectura[i][ESCRITURA], &v,sizeof(visibilidades));
                    aux++;
                }
                if(distancia >=rangoDiscos[o.cantDiscos-1] && i == o.cantDiscos-1){ // caso para el ultimo rango 
                    write(arregloPipesLectura[i][ESCRITURA], &v,sizeof(visibilidades));
                    aux++;
                }
                
            }
            fclose(fp);
            
            waitpid(pid,&status,0);

            close(arregloPipesEscritura[i][ESCRITURA]); // para evitar errores con los hijos cerramos el pipe
            read(arregloPipesEscritura[i][LECTURA],&r,sizeof(resultado)); // recibimos los resultados calculados por el hijo
            close(arregloPipesEscritura[i][LECTURA]);

            // escribimos los resultados en el archivo de texto.
            fprintf(salida,"Disco %d\n",i+1); 
            fprintf(salida,"Media Real: %Lf\n",r.mediaReal);
            fprintf(salida,"Media Imaginaria: %Lf\n",r.mediaImaginaria);
            fprintf(salida,"Potencia: %Lf\n",r.potencia);
            fprintf(salida,"Ruido total: %Lf\n",r.ruidoTotal);
            
            
            
        }else{ // soy el hijo
            close(arregloPipesEscritura[i][LECTURA]); // cerramos el lado de lectura, puesto que no lo leeremos
            dup2(arregloPipesEscritura[i][ESCRITURA],STDOUT_FILENO); // hacemos una copia del pipe al stdout
            close(arregloPipesEscritura[i][ESCRITURA]);

            close(arregloPipesLectura[i][ESCRITURA]); // cerramos el lado de escritura 
            dup2(arregloPipesLectura[i][LECTURA],STDIN_FILENO); // hacemos una copia del pipe al stdin
            close(arregloPipesLectura[i][LECTURA]);

            // transformo el total de visibilidades para el respectivo disco a string
            sizeCNum = snprintf(NULL, 0, "%d",cantidadVisibilidades[i]);
            cNum = malloc(sizeCNum + 1);
            snprintf(cNum,sizeCNum+1,"%d",cantidadVisibilidades[i]);
            

           
            // le pasamos el total de visibilidades al vis por medio de un parametro de entrada.
            execl("vis",cNum,NULL);
            printf("Soy el hijo de pid %d, procese %d visibilidades",getpid(),cantidadVisibilidades[i]);

        }
    }
    // liberamos memoria y cerramos archivos
    fclose(salida);
    free(cantidadVisibilidades);
    free(rangoDiscos);
    for (int i=0;i<o.cantDiscos;i++){ //Se libera la memoria de los pipes
        free(arregloPipesEscritura[i]);
        free(arregloPipesLectura[i]);
    }

    return 0;
}