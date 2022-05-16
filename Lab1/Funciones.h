//#ifndef Funciones_h_
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

typedef struct lineaDeComando{
    char archivoVisibilidades[40];
    char archivoSalida[40];
    int cantDiscos;
    int anchoDisco;
    int ptrcsl; // variable para saber si se imprime por pantalla o no
}entradaComando;

typedef struct structResultado{
    float mediaReal;
    float mediaImaginaria;
    float potencia;
    float ruidoTotal;
}resultado;

int cuenta_lineas(char* nombreArchivo);
visibilidades* leer_visibilidades(char *nombreArchivo);
void verificador_entradas(entradaComando entrada);
int* arreglo_visibilidades_por_disco(int cantidadVisibilidades, int cantidadDiscos);
int** crea_pipes_lectura(int cantidadDiscos);
int** crea_pipes_escritura(int cantidadDiscos);

//#endif