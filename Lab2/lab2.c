/*
Integrantes:
            Cristhofer Parada RUT: 20387754-4
            Benjamin Parra RUT: 19741126-0

Profesor Catedra: Fernado Rannou

Ayudante: Ricardo Hasbun
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "Funciones.h"


/*
Variables Globales
*/
FILE *archivoEntrada; // archivo de lectura global
// mmutex de escritura y de lectura
pthread_mutex_t mLectura; 
pthread_mutex_t mEscritura;
entradaComando entrada;// variable global donde se almacena los datos obtenidos por la linea de comandos
resultado  *discos; // variable con los datos de cada disco 
int *rangoDiscos; // variable donde se hace el calculo de los radios de los discos
int key = 0; // semaforo 


/*
 * Function:  ejecucion
 * --------------------
 * Rutina que se le da a las hebras para que cada una la ejecute, asegurandose de que exista concurrencia y teniendo cuidado con las zonas criticas 
 *  entrada: void *unused
 *  retorno: void
 */
void *ejecucion(void *unused);

int main(int argc, char *argv[]){
    // definicion de algunas variables
    int opt;
    FILE *archivoSalida;
    char fila[1000];

    // obtenemos los parametros por entrada de la terminal 
    while ((opt = getopt(argc,argv, "i:o:d:n:h:c:b"))!= -1){
        switch (opt){
            case 'i': // caso para el nombre del archivo de entrada
                strcpy(entrada.archivoEntrada,optarg);
                break;
            case 'o': // caso para el nombre del archivo de salida
                strcpy(entrada.archivoSalida,optarg);
                break;
            case 'd': // caso para el ancho del disco 
                entrada.anchoDisco = atoi(optarg);
                break;
            case 'n': // caso para la cantidad de discos 
                entrada.cantDiscos = atoi(optarg);
                break;
            case 'h': // caos para la cantidad de hebras
                entrada.cantHebras = atoi(optarg);
                break;
            case 'c': // caso para el tamanyo del chunk 
                entrada.chunk = atoi(optarg);
                break;
            case 'b': // caso para la opcion de si imprimir por pantalla o no 
                entrada.flag = 1;
                break;
        }
    }

    verificador_entradas(entrada); // verificamos que las entradas sean las correctas
    // asignamos memoria al  arreglo de rango discos donde sus valores seran de tipo 0, 10, 20, 30rrrr
    rangoDiscos = (int*)malloc(sizeof(int)*entrada.cantDiscos); 
    for (int i = 0; i < entrada.cantDiscos; i++) {
        rangoDiscos[i] = entrada.anchoDisco*i;
    }

    // asignamos memoria al arreglo de los discos
    discos = (resultado*)malloc(sizeof(resultado)*entrada.cantDiscos);
    // seteamos sus valores inicdiales en 0
    for (int i = 0; i < entrada.cantDiscos; i++) {
        discos[i].mediaReal = 0;
        discos[i].mediaImaginaria = 0;
        discos[i].potencia = 0;
        discos[i].ruidoTotal = 0;
    }
    // abrimos el archivo de entrada en forma de lectura d
    archivoEntrada = fopen(entrada.archivoEntrada,"r");

    // inicializamos los mutex
    pthread_mutex_init(&mLectura, NULL);
    pthread_mutex_init(&mEscritura, NULL);  

    // creamos el arreglo de los atributos de las hebras

    pthread_attr_t attr[entrada.cantHebras];

    // inicicializamos dichos atributos
    for (int i = 0; i < entrada.cantHebras; i++) {
        pthread_attr_init(&attr[i]);
    }

    // creamos el arreglo con las hebras

    pthread_t thread[entrada.cantHebras];
    // ejecutamos cada una de las hebras con la rutina llamada ejecucion
    for (int i = 0; i < entrada.cantHebras; i++) {
        pthread_create(&thread[i], &attr[i],ejecucion,NULL);
        
    }
    // una vez terminado cerramos el archivo de entrada
    fclose(archivoEntrada);
    // abirmos el archivo de salida para escribir sus datos
    archivoSalida = fopen(entrada.archivoSalida,"w");
    
    // recorremos el arreglo del a cantidad de discos y calculamos algunas de las medidas
    for (int i = 0; i < entrada.cantDiscos; i++) {
        discos[i].potencia = sqrt(discos[i].potencia);
        discos[i].mediaReal = discos[i].mediaReal / discos[i].contador;
        discos[i].mediaImaginaria = discos[i].mediaImaginaria / discos[i].contador;
        fprintf(archivoSalida,"Disco %d:\n",i+1);
        fprintf(archivoSalida,"Media real: %Lf\n",discos[i].mediaReal);
        fprintf(archivoSalida,"Media imaginaria: %Lf\n",discos[i].mediaImaginaria);
        fprintf(archivoSalida,"Potencia: %Lf\n",discos[i].potencia);
        fprintf(archivoSalida,"Ruido Total: %Lf\n",discos[i].ruidoTotal);
    }

    
    fclose(archivoSalida);
    // consultamos si dentro de la linea de comandos se ejecuto la opcion de -b, de ser asi, printeamos la salida por pantalla
    if (entrada.flag == 1){
        printearResultados(entrada.archivoSalida);
    }
    // liberamos la memoria del rango de discos y de los discos
    free(rangoDiscos);
    free(discos);
    // eliminamos los mutex de lectura y de escritura
    pthread_mutex_destroy(&mLectura);
    pthread_mutex_destroy(&mEscritura);

    

    

    return 0;
}

void *ejecucion(void *unused){
    // ppreguntamos si hemos llegado al final del archivo de lectura
    if (!feof(archivoEntrada)){
        // cerramos el mutex de lectura,
        pthread_mutex_lock(&mLectura);
        // seteamos el contador de lineas en cero
        int contador = 0;
        // hacemos el ciclo mientras el contador sea menor a la cantidad de lineas, key sea distinto a 1 y no sea el final del archivo
        while (contador < entrada.chunk && key != 1 && !feof(archivoEntrada)){
            // si key == 0
            if (key == 0){
                // leemos una visibilidad del archivo
                visibilidades actual;
                if(fscanf(archivoEntrada,"%f,%f,%f,%f,%f", &actual.ejeU,&actual.ejeV,&actual.valorReal,&actual.valorImaginario,&actual.ruido)){

                    // ubicamos en que disco se encuentra la visibilidad actual
                    double distancia = sqrt(pow(actual.ejeU,2) + pow(actual.ejeV,2));
                    int pos;
                    // recorremos el rango de los discos para ubicar la distancia actual dentro de su radio
                    for (int i = 0; i < entrada.cantDiscos ; i++) {
                        // por ejemplo preguntamos si la distancia esta entre  0 y 10 y ademas que no sea el final del arreglo
                        if(distancia >= rangoDiscos[i] && distancia < rangoDiscos[i+1] && i+1 < entrada.cantDiscos ){
                            pos = i;
                        }
                        // caso para el final del arreglo donde distnacia > ultimo radio del disco
                        if (distancia >= rangoDiscos[entrada.cantDiscos-1] && i == entrada.cantDiscos-1){
                            pos = i;
                        }
                    }
                    // aumentamos el contador de lineas
                    contador++;
                    pthread_mutex_lock(&mEscritura); // como vamos a escribir dentro de una variable, cerramos el mutex de escritura
                    // guardamos las visibilidades leidas 
                    discos[pos].contador++;
                    discos[pos].mediaReal = discos[pos].mediaReal + actual.valorReal;
                    discos[pos].mediaImaginaria = discos[pos].mediaImaginaria + actual.valorImaginario;
                    discos[pos].ruidoTotal = discos[pos].ruidoTotal + actual.ruido;
                    discos[pos].potencia = discos[pos].potencia + (sqrt(pow(actual.valorReal,2) + pow(actual.valorImaginario,2)));
                    // terminada la manipulacion de la variable globalm abrimos el mutex de escritur
                    pthread_mutex_unlock(&mEscritura); 
                }
            }else{
                key = 0;
            }
        }
        pthread_mutex_unlock(&mLectura); // terminada la lectura abrimos el mutex de lecutra
        ejecucion(NULL);
    }else{ // si ya llegamos el final del archivo entonces cerramos el hilo
        pthread_exit(NULL);
    }
    // si aun quedan lineas, cerramos la hebra 
    pthread_exit(NULL);
    
}