/* C Program to count the Number of Lines in a Text File */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_FILE_NAME 100


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

int cuenta_lineas(char* nombreArchivo);
visibilidades* leer_visibilidades(char *nombreArchivo);
int main(int argc, char *argv[]){   
    entradaComando opciones;
    opciones.ptrcsl = 0;
    int opt;

    while((opt = getopt(argc,argv,":i:o:d:n:b")) != -1){
        switch (opt){
            case 'i':
                strcpy(opciones.archivoVisibilidades,optarg);
                break;
            case 'o':
                strcpy(opciones.archivoSalida,optarg);
                break;
            case 'd':
                opciones.cantDiscos = atoi(optarg);
                break;
            case 'n':
                opciones.anchoDisco = atoi(optarg);
                break;
            case 'b':
                opciones.ptrcsl = 1;
                break;
        }
    }

    printf("\n%s %s %d %d %d\n",opciones.archivoVisibilidades,opciones.archivoSalida,
            opciones.cantDiscos,opciones.anchoDisco,opciones.ptrcsl);

	
    
	return 0;
}
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



