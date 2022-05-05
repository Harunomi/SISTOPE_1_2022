/* C Program to count the Number of Lines in a Text File */
#include <stdio.h>
#include <stdlib.h>
#define MAX_FILE_NAME 100


//Estructura para representar una visibilidad
struct visibilidades{
    float ejeU;
    float ejeV;
    float valorReal;
    float valorImaginario;
    float ruido;
};typedef struct visibilidades visibilidades;

int cuenta_lineas(char* nombreArchivo);
visibilidades* leer_visibilidades(char *nombreArchivo);
int main()
{
	char filename[MAX_FILE_NAME];
    int cantidadLineas;
	printf("Enter file name: ");
	scanf("%s", filename);
    
    cantidadLineas = cuenta_lineas(filename);
    
	printf("El archivo %s tiene  %d lineas \n ", filename, cantidadLineas);//esto es un verificador
    visibilidades* listaVisibilidades;
    listaVisibilidades = leer_visibilidades(filename);

    for (int i = 0; i < 10; i++)
    {
        printf("PARA i = %i dato1:%f dato2:%f dato3:%f\n dato4:%f\n dato5:%f\n",i,listaVisibilidades[i].ejeU, listaVisibilidades[i].ejeV, listaVisibilidades[i].valorReal, listaVisibilidades[i].valorImaginario, listaVisibilidades[i].ruido);
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
    visibilidades *listaVisibilidades = (visibilidades*)malloc(sizeof(struct visibilidades) * cantidadVisibilidades);
    if (archivo = fopen(nombreArchivo, "r")){
		//fgets(buffer, 1024, archivo);
		while (fscanf(archivo, "%f,%f,%f,%f,%f", &listaVisibilidades[i].ejeU, &listaVisibilidades[i].ejeV, &listaVisibilidades[i].valorReal, &listaVisibilidades[i].valorImaginario, &listaVisibilidades[i].ruido) != EOF){
			++i;
		}
		fclose(archivo);
	}
    return listaVisibilidades;


}


