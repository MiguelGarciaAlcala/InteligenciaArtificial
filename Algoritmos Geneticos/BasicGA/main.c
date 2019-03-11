#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "agenetico.h"

/**
    ALGORITMO GENÉTICO BÁSICO
    Este programa ejemplifica la implementación de un AG para encontrar un
    individuo cuyos genes coinciden con los elementos de un arreglo de enteros
    entre 0 y 9 definido por el usuario.

    Los archivos de encabezado requeridos para la ejecución el algoritmo son:
        a) individuo.h
           Define las funciones necesarias para manipular estructuras de
           tipo Individuo; crear individuos, imprimirlos, copiarlos, mutar
           individuos, cruzarlos, etc.
        b) poblacion.h
           Sirve para manipular estructuras de tipo Poblacion; crear nuevas
           poblaciones, imprimirlas, calcular su aptitud, mutarlas, etc.
        c) agenetico.h
           Controla los parámetros del AG, tales como el tamaño de la población,
           longitud de los individuos, porcentaje de torneo, probabilidades de
           cruce y mutación, así como el porcentaje de elitismo.

    NOTA: se incluyen varios métodos de selección: por torneo determinista, por
          torneo probabilista y por ruleta. El AG puede tener un comportamiento
          distinto de acuerdo al tipo de selección utilizado.

          El tipo de cruce es uniforme y la mutación consiste en asignar un
          entero aleatorio entre 0 y 9 como alelo de un individuo dado.

    Autor: Luis Miguel García Alcalá
    Fecha: Marzo de 2019
**/

int main()
{
    // Define una semilla en función del tiempo.
    srand(time(NULL));

    // Ejecuta el AG con un máximo de 100 generaciones.
    ejecutarAlgoritmoGenetico(100);

    return 0;
}
