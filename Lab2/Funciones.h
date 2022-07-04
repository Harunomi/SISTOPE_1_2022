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
    int contador;
}resultado;



/*
 * Function:  verificador_entradas
 * --------------------
 * Comprueba que los datos almacenados en la estructura de archivos estè correcta
 *  entrada: estructura con los datos de entrada
 *  retorno: void
 */
void verificador_entradas(entradaComando e);
/*
 * Function:  pintear resultados
 * --------------------
 * funcion que permite mostrar por pantallas los datos obtenidos por los discos=
 *  entrada: un char con el nombr edel archivo
 *  retorno: void
 */
void printearResultados(char *nombreArchivo);




#endif