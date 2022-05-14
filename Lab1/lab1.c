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
int verificador_entradas(entradaComando entrada);
int* arreglo_visibilidades_por_disco(int cantidadVisibilidades, int cantidadDiscos);

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
    /*
    //Se hace llamado a metodo que verifica las condiciones 
    int verificador = verificador_entradas(opciones);
    if (verificador = 1)
    {
        return 0;
    }*/
    

    printf("\n%s %s %d %d %d\n",opciones.archivoVisibilidades,opciones.archivoSalida,
            opciones.cantDiscos,opciones.anchoDisco,opciones.ptrcsl);

    int cantidadTotalVisibilidades = cuenta_lineas(opciones.archivoVisibilidades);
    //se crea arreglo con las cantidades de visibilidades que tendrà cada disco
    int* arreglos = arreglo_visibilidades_por_disco(cantidadTotalVisibilidades, opciones.cantDiscos);

    for (int i = 0; i < opciones.cantDiscos; i++)
    {
        printf("ID= %d numero %d", i,arreglos[i]);
    }
    
    

	
    
    
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

/*
 * Function:  verificador_entradas
 * --------------------
 * Comprueba que los datos almacenados en la estructura de archivos estè correcta
 *  entrada: estructura con los datos de entrada
 *  retorno: void
 */
int verificador_entradas(entradaComando entrada){
    int output = 0;
     if (entrada.anchoDisco <= 0){
        printf("El ancho del disco debe ser mayor a 0.\n");
        output =+ 1;
        return output;
    }

    if (entrada.cantDiscos <= 0){
        printf("La cantidad de discos debe ser mayor a 0.\n");
        output =+ 1;
        return output;
    }

    FILE *archivo=fopen(entrada.archivoVisibilidades,"r");
    if(archivo == NULL){ 
        printf("El archivo ingresado no ha sido localizado.\n");
        output =+ 1;
        return output;
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

