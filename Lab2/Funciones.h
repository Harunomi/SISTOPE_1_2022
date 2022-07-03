#ifndef Funciones_h_
#define Funciones_h_
#include <stdio.h>

//Estructura para representar una visibilidad
typedef struct visibilidadesStruct{
    float ejeU;
    float ejeV;
    float valorReal;
    float valorImaginario;
    float ruido;
}visibilidades;

// Estructura para guardar los datos de entrada
typedef struct lineaDeComando{
    char archivoEntrada[40];
    char archivoSalida[40];
    int cantDiscos;
    int anchoDisco;
    int flag; // variable para saber si se imprime por pantalla o no
    int cantHebras;
    int chunk;
}entradaComando;

// Estructura en donde se guardan los resultados obtenidos por cada disco.
typedef struct structResultado{
    long double mediaReal;
    long double mediaImaginaria;
    long double potencia;
    long double ruidoTotal;
}resultado;

/*
 * Function:  cuenta_lineas 
 * --------------------
 * cuenta la cantidad de lineas que contiene un archivo
 *
 *  nombreArchivo: es el nombre del archivo el cual se abrirá y se procederá a contar sus lineas
 *
 *  retorno: cantidad de lineas que tiene un archivo
 */
int cuenta_lineas(char *nombreArchivo);