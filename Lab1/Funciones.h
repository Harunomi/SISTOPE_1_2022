#ifndef Funciones_h_
#define Funciones_h_
#include<stdio.h>

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
    char archivoVisibilidades[40];
    char archivoSalida[40];
    int cantDiscos;
    int anchoDisco;
    int ptrcsl; // variable para saber si se imprime por pantalla o no
}entradaComando;

// Estructura en donde se guardan los resultados obtenidos por cada disco.
typedef struct structResultado{
    long double mediaReal;
    long double mediaImaginaria;
    long double potencia;
    long double ruidoTotal;
}resultado;

int cuenta_lineas(char *nombreArchivo);

void verificador_entradas(entradaComando entrada);

resultado calcular(visibilidades* v,int total);

int *totalVisibilidades(char *nombreArchivo,int totalLineas,int cantDiscos,int *rangoDiscos);
#endif