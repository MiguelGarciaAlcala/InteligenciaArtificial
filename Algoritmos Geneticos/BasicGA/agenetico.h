#ifndef AGENETICO_H_INCLUDED
#define AGENETICO_H_INCLUDED

#include <stdio.h>
#include "poblacion.h"

#define TAM_POBLACION 30
#define TAM_INDIVIDUO 10
#define PORC_TORNEO 30
#define PROB_CRUCE 0.5
#define PROB_MUTACION 0.015
#define PORC_ELITISMO 10

/** Prototipos **/
void ejecutarAlgoritmoGenetico(int);
int solucionEncontrada(Poblacion *);

/** Implementación **/
void ejecutarAlgoritmoGenetico(int maxgen)
{
    Poblacion * poblacion = nuevaPoblacion(TAM_POBLACION, TAM_INDIVIDUO, 1);
    imprimirPoblacion(poblacion);

    Poblacion * tmp = NULL;
    int i;

    for(i = 0; i < maxgen; ++i)
    {
        printf("Generación %d, fitness: %g\n", i, poblacion->aptitud);

        if(solucionEncontrada(poblacion))
        {
            printf("\nSolución en %d generaciones\n", i);
            imprimirPoblacion(poblacion);
            break;
        }

        tmp = nuevaGeneracion(poblacion, PORC_TORNEO, PORC_ELITISMO, PROB_CRUCE, PROB_MUTACION);
        copiarPoblacion(poblacion, tmp);

        destruirPoblacion(&tmp);
    }

    if(!solucionEncontrada(poblacion))
        printf("\nNo se encontraron soluciones\n");

    destruirPoblacion(&poblacion);
}

int solucionEncontrada(Poblacion * poblacion)
{
    int i;
    for(i = 0; i < poblacion->tamanio; ++i)
    {
        if((poblacion->individuos[i])->aptitud == 1.0)
            return 1;
    }

    return 0;
}

#endif // AGENETICO_H_INCLUDED
