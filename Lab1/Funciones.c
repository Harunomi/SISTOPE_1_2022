#include <stdio.h>
#include <stdlib.h>
#include "Funciones.h"
#include "math.h"

/*
 * Function:  cuenta_lineas 
 * --------------------
 * cuenta la cantidad de lineas que contiene un archivo
 *
 *  nombreArchivo: es el nombre del archivo el cual se abrirá y se procederá a contar sus lineas
 *
 *  retorno: cantidad de lineas que tiene un archivo
 */
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
/*
 * Function:  leer_visibilidades
 * --------------------
 * lee el archivo y almacena los valores en un arreglo de estructuas
 *
 *  nombreArchivo: es el nombre del archivo el cual se abrirá y se procederá a contar sus lineas
 *  cantidadVisibilidades: cantidad de visibilidades existentes en el archivo
 *  retorno: cantidad de lineas que tiene un archivo
 */
visibilidades* leer_visibilidades(char *nombreArchivo){
    FILE* archivo;
    int  i, cantidadVisibilidades;
    cantidadVisibilidades = cuenta_lineas(nombreArchivo);
    i = 0;
    //char buffer[1024];
    visibilidades *listaVisibilidades = (visibilidades*)malloc(sizeof(visibilidades) * cantidadVisibilidades);
    if (archivo = fopen(nombreArchivo, "r")){
		//fgets(buffer, 1024, archivo);
		while (fscanf(archivo, "%f,%f,%f,%f,%f", &listaVisibilidades[i].ejeU, &listaVisibilidades[i].ejeV, &listaVisibilidades[i].valorReal, &listaVisibilidades[i].valorImaginario, &listaVisibilidades[i].ruido) != EOF){
			++i;
		}
		fclose(archivo);
	}
    return listaVisibilidades;
}

/*
 * Function:  verificador_entradas
 * --------------------
 * Comprueba que los datos almacenados en la estructura de archivos estè correcta
 *  entrada: estructura con los datos de entrada
 *  retorno: void
 */
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

/*
 * Function:  arreglo_visibilidades_por_disco
 * --------------------
 * Crea un arreglo con la cantidad de cuantas visibilidades debe procesar cada disco (hijo)
 *  entrada: cantidad total de visibilidades en el archivo, cantidad de discos (hijos)
 *  retorno: arreglo de enteros
 */
int* arreglo_visibilidades_por_disco(int cantidadVisibilidades, int cantidadDiscos){
    int cantidadPorDisco = cantidadVisibilidades / cantidadDiscos;
    int * arregloCantidades = malloc(sizeof(int)*cantidadDiscos+1);
    if (cantidadVisibilidades%cantidadDiscos == 0)
    {
        for (int i = 0; i < cantidadDiscos; i++)
        {
            arregloCantidades[i] = cantidadPorDisco;
        }
        
    }else{
        for (int i = 0; i < cantidadDiscos - 1; i++)
        {
            arregloCantidades[i] = cantidadPorDisco;
        }
        arregloCantidades[cantidadDiscos-1] = cantidadPorDisco + 1;
        
    }
    return arregloCantidades;
    
}

/*
 * Function:  crea_pipes_lectura(
 * --------------------
 * Crea un arreglo con los pipes para realizar la lectura entre padre e hijo
 *  entrada: cantidad de discos (hijos)
 *  retorno: arreglo de pipes
 */
int** crea_pipes_lectura(int cantidadDiscos){
    int ** pipesLectura = (int**)malloc(sizeof(int *)*cantidadDiscos);
    for (int i = 0; i < cantidadDiscos; i++)
    {
        pipesLectura[i] = (int*)malloc(sizeof(int)*2);
        pipe(pipesLectura[i]);
    }
    return pipesLectura;
}

/*
 * Function:  crea_pipes_escritura(
 * --------------------
 * Crea un arreglo con los pipes para realizar la escritura entre padre e hijo
 *  entrada: cantidad de discos (hijos)
 *  retorno: arreglo de pipes
 */
int** crea_pipes_escritura(int cantidadDiscos){
    int ** pipesEscritura = (int**)malloc(sizeof(int *)*cantidadDiscos);
    for (int i = 0; i < cantidadDiscos; i++)
    {
        pipesEscritura[i] = (int*)malloc(sizeof(int)*2);
        pipe(pipesEscritura[i]);
    }
    return pipesEscritura;
}