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
int cuenta_lineas(char *nombreArchivo){
    int cantidadLineas;
    char variableContador; //Variable auxiliara para recorrer archivo
    cantidadLineas = 0;
    FILE *fp;
    fp = fopen(nombreArchivo,"r");
    if (nombreArchivo == NULL){
        printf("No se pudo abrir el archivo %s", nombreArchivo);
		return 0;
    }else{
        for (variableContador = getc(fp); variableContador != EOF; variableContador = getc(fp))
        {
            if (variableContador == '\n') {// Aumenta cada vez que encuentra un salto de lineas
            cantidadLineas = cantidadLineas + 1;
            }
        }
    }
    fclose(fp);
    return cantidadLineas;
}
void verificador_entradas(entradaComando entrada){
     if (entrada.anchoDisco <= 0){
        printf("ERROR: El ancho del disco debe ser mayor a 0.\n");
        exit(1);
    }

    if (entrada.cantDiscos <= 0){
        printf("ERROR: La cantidad de discos debe ser mayor a 0.\n");
        exit(1);
    }

    FILE *archivo=fopen(entrada.archivoVisibilidades,"r");
    if(archivo == NULL){ 
        printf("ERROR: El archivo ingresado no ha sido localizado.\n");
        exit(1);
    }
    fclose(archivo);   
    

}

int *totalVisibilidades(char *nombreArchivo,int totalLineas,int cantDiscos,int *rangoDiscos){
    visibilidades v;
    float distancia;
    int counter;
    int counter2;
    int *cantidadVisibilidades = (int*)malloc(sizeof(int)*cantDiscos);
    for (int i = 0; i < cantDiscos; i++) {
        cantidadVisibilidades[i] = 0;
    }
    FILE *fp;
    for (int i = 0; i < cantDiscos; i++) {
        counter = 0;
        counter2 = 0;
        fp = fopen(nombreArchivo,"r");
        for (int k = 0; k < totalLineas; k++) {
            fscanf(fp,"%f,%f,%f,%f,%f",&v.ejeU,&v.ejeV,&v.valorReal,&v.valorImaginario,&v.ruido);
            distancia = sqrt(pow(v.ejeU,2) + pow(v.ejeV,2));
            // determino si esa distancia corresponde al disco (proceso) actual
            if(distancia>=rangoDiscos[i] && distancia < rangoDiscos[i+1] && i+1 < cantDiscos){   
                // si corresponde al proceso actual, entonces lo mando por el pipe
                counter++;
            }
            if(distancia >=rangoDiscos[cantDiscos-1]){
                counter2++;
            }
        }
        cantidadVisibilidades[i] = counter;
        if(i == cantDiscos-1){
            cantidadVisibilidades[cantDiscos-1] = counter2;
        }
        fclose(fp);
    }
    return cantidadVisibilidades;
    
}

int main(int argc, char *argv[]){   
    entradaComando o;
    visibilidades v;
    visibilidades out;
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
    // creo un arreglo con los rangos del disco
    int *rangoDiscos = (int*)malloc(sizeof(int)*o.cantDiscos); 
    for (int i = 0; i < o.cantDiscos; i++) {
        rangoDiscos[i] = o.anchoDisco*i;
    }

    // creo un arreglo para guardar el total de visibilidades que le toca a cada disco.
    int *cantidadVisibilidades = (int*)malloc(sizeof(int)*o.cantDiscos);
    //lo inicializo en cero
    //for (int i = 0; i < o.cantDiscos; i++) {
    //    cantidadVisibilidades[i] = 0;
   // }

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
    
    

    for (int i = 0; i < o.cantDiscos; i++) {
        pid = fork();
        
        if (pid != 0){ // soy el padre
            fp = fopen(o.archivoVisibilidades,"r");
            printf("   %d\n",i);
            for (int j = 0; j < totalLineas ; j++) {
                // leo la linea del codigo
                fscanf(fp,"%f,%f,%f,%f,%f",&v.ejeU,&v.ejeV,&v.valorReal,&v.valorImaginario,&v.ruido);
                // calculo su distancia
                distancia = sqrt(pow(v.ejeU,2) + pow(v.ejeV,2));
                // determino si esa distancia corresponde al disco (proceso) actual
                if(distancia>=rangoDiscos[i] && distancia < rangoDiscos[i+1] && i+1 < o.cantDiscos){   
                    // si corresponde al proceso actual, entonces lo mando por el pipe
                    write(arregloPipesLectura[i][ESCRITURA], &v, sizeof(visibilidades));   
                    //printf("%d\n",j);                 
                }
                if(distancia >=rangoDiscos[o.cantDiscos-1]){
                    write(arregloPipesLectura[o.cantDiscos-1][ESCRITURA], &v, sizeof(visibilidades));
                }
                
            }
            fclose(fp);

            waitpid(pid,&status,0);
            close(arregloPipesEscritura[i][ESCRITURA]);
            
            for (int k = 0; k < cantidadVisibilidades[i]; k++) {
                read(arregloPipesEscritura[i][LECTURA], &out,sizeof(visibilidades));
                printf("%d %f %f %f %f %f\n",k,out.ejeU,out.ejeV,out.valorReal,out.valorImaginario,out.ruido);
            }
            printf("termine\n");
            close(arregloPipesEscritura[i][LECTURA]);
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