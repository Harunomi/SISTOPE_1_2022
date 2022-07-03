#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <unistd.h>
#include "Funciones.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>




/*
 * Function:  verificador_entradas
 * --------------------
 * Comprueba que los datos almacenados en la estructura de archivos estè correcta
 *  entrada: estructura con los datos de entrada
 *  retorno: void
 */
void verificador_entradas(entradaComando e){
     if (e.anchoDisco <= 0){
        printf("ERROR: El ancho del disco debe ser mayor a 0.\n");
        exit(1);
    }

    if (e.cantDiscos <= 0){
        printf("ERROR: La cantidad de discos debe ser mayor a 0.\n");
        exit(1);
    }

    if (e.chunk <= 0){
        printf("ERROR: El valor de chunk debe ser mayor que 0");
        exit(1);
    }

    if (e.cantHebras <= 0){
        printf("ERROR: El valor de la cantidad de hebras debe ser mayor a 0");
        exit(1);
    }

    FILE *archivo=fopen(e.archivoEntrada,"r");
    if(archivo == NULL){ 
        printf("ERROR: El archivo ingresado no ha sido localizado.\n");
        exit(1);
    }
    fclose(archivo);   
    
}
/*
 * Function:  printearResultados
 * --------------------
 * Permite dada una bandera dada, mostrar los resultados obtenidos por los calculas en pantalla
 *  entrada: un puntero de tipo char
 *  retorno: void
 */
void printearResultados(char *nombreArchivo){
    FILE *fp = fopen(nombreArchivo,"r");
    char fila[1000];
    while(!feof(fp)){
        fscanf(fp," %[^\n]",fila);
        printf("%s\n",fila);
    }
    fclose(fp);
}

