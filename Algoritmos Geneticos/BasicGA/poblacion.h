#ifndef POBLACION_H_INCLUDED
#define POBLACION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "individuo.h"

// Arreglo objetivo
int objetivo[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

typedef struct
{
    Individuo ** individuos;
    int tamanio;
    int genes_ind;
    double aptitud;
} Poblacion;

/** Prototipos **/

// Operaciones básicas.
Poblacion * nuevaPoblacion(int, int, int);
void copiarPoblacion(Poblacion *, Poblacion *);
void destruirPoblacion(Poblacion **);
void imprimirPoblacion(Poblacion *);

// Operaciones AG.
Individuo * seleccionTorneoDeterminista(Poblacion *, int);
Individuo * seleccionTorneoProbabilista(Poblacion *);
Individuo * seleccionRuleta(Poblacion *);
void calcularFitnessPoblacion(Poblacion *);
void mutarPoblacion(Poblacion *, double);
Poblacion * nuevaGeneracion(Poblacion *, int, int, double, double);
int * obtenerMejores(Poblacion *, int);

/** Implementacion **/

Poblacion * nuevaPoblacion(int tamanio, int no_genes, int inicializar)
{
    Poblacion * nueva = (Poblacion *) malloc(sizeof(Poblacion));
    nueva->individuos = (Individuo **) malloc(tamanio * sizeof(Individuo *));
    nueva->tamanio = tamanio;
    nueva->genes_ind = no_genes;
    nueva->aptitud = 0.0;

    int i;
    for(i = 0; i < tamanio; ++i)
    {
        nueva->individuos[i] = nuevoIndividuo(no_genes, inicializar);

        if(inicializar)
        {
            calcularFitnessIndividuo(nueva->individuos[i], objetivo);
            nueva->aptitud += (nueva->individuos[i])->aptitud;
        }
    }

    return nueva;
}

void copiarPoblacion(Poblacion * destino, Poblacion * fuente)
{
    if(destino && fuente)
    {
        if(destino->tamanio == fuente->tamanio)
        {
            destino->genes_ind = fuente->genes_ind;
            destino->aptitud = fuente->aptitud;

            int i;
            for(i = 0; i < destino->tamanio; ++i)
                copiarIndividuo(destino->individuos[i], fuente->individuos[i]);
        } else
            printf("El tamaño de las poblaciones no coincide: %d != %d", destino->tamanio, fuente->tamanio);
    }
}

void destruirPoblacion(Poblacion ** poblacion)
{
    if(poblacion)
    {
        if(*poblacion)
        {
            if((*poblacion)->individuos)
            {
                int i;
                for(i = 0; i < (*poblacion)->tamanio; ++i)
                    destruirIndividuo(&((*poblacion)->individuos[i]));

                free((*poblacion)->individuos);
                (*poblacion)->individuos = NULL;
            }

            free(*poblacion);
            *poblacion = NULL;
        }
    }
}

void imprimirPoblacion(Poblacion * poblacion)
{
    if(poblacion)
    {
        printf("POBLACIÓN\n");

        int i;
        for(i = 0; i < poblacion->tamanio; ++i)
            imprimirIndividuo(poblacion->individuos[i]);

        printf("* Fitness: %g\n", poblacion->aptitud);
    }
}

Individuo * seleccionTorneoDeterminista(Poblacion * poblacion, int porc_torneo)
{
    int tam_torneo = (int) ((double) (porc_torneo * poblacion->tamanio) / 100.0);
    Poblacion * pTorneo = nuevaPoblacion(tam_torneo, poblacion->genes_ind, 0);

    double rnd;
    int index, maximo = 0, i;

    for(i = 0; i < tam_torneo; ++i)
    {
        rnd =(double) rand() / (double) ((unsigned)RAND_MAX + 1);
        index = (int) (rnd * poblacion->tamanio);

        copiarIndividuo(pTorneo->individuos[i], poblacion->individuos[index]);

        if((pTorneo->individuos[i])->aptitud > (pTorneo->individuos[maximo])->aptitud)
            maximo = i;
    }

    Individuo * elegido = nuevoIndividuo(pTorneo->genes_ind, 0);
    copiarIndividuo(elegido, pTorneo->individuos[maximo]);

    //imprimirPoblacion(pTorneo);
    //imprimirIndividuo(elegido);

    destruirPoblacion(&pTorneo);

    return elegido;
}

Individuo * seleccionTorneoProbabilista(Poblacion * pob)
{
    double rnd1 = (double) rand() / (double) ((unsigned) RAND_MAX + 1);
    double rnd2 = (double) rand() / (double) ((unsigned) RAND_MAX + 1);
    int ind1 = (int) (rnd1 * pob->tamanio);
    int ind2 = (int) (rnd2 * pob->tamanio);

    Individuo * elegido = nuevoIndividuo(pob->genes_ind, 0);

    int mejor = ind1;
    int peor = ind2;
    if((pob->individuos[ind1])->aptitud < (pob->individuos[ind2])->aptitud)
    {
        peor = ind1;
        mejor = ind2;
    }

    double rnd = (double) rand() / (double) ((unsigned) RAND_MAX + 1);
    if(rnd > 0.5)
        copiarIndividuo(elegido, pob->individuos[mejor]);
    else
        copiarIndividuo(elegido, pob->individuos[peor]);

    return elegido;
}

Individuo * seleccionRuleta(Poblacion * poblacion)
{
    double rnd = (double) rand() / (double) ((unsigned) RAND_MAX + 1);
    int S = (int) (rnd * poblacion->aptitud);

    Individuo * elegido = nuevoIndividuo(poblacion->genes_ind, 0);
    double acumulado = 0.0;
    int i;

    for(i = 0; i < poblacion->tamanio; ++i)
    {
        acumulado += (poblacion->individuos[i])->aptitud;

        if(acumulado >= S)
        {
            copiarIndividuo(elegido, poblacion->individuos[i]);
            break;
        }
    }

    return elegido;
}

void calcularFitnessPoblacion(Poblacion * poblacion)
{
    if(poblacion)
    {
        poblacion->aptitud = 0.0;

        int i;
        for(i = 0; i < poblacion->tamanio; ++i)
        {
            calcularFitnessIndividuo(poblacion->individuos[i], objetivo);
            poblacion->aptitud += (poblacion->individuos[i])->aptitud;
        }
    }
}

void mutarPoblacion(Poblacion * poblacion, double prob_mutacion)
{
    if(poblacion)
    {
        int i;
        for(i = 0; i < poblacion->tamanio; ++i)
            mutarIndividuo(poblacion->individuos[i], prob_mutacion);
    }
}

Poblacion * nuevaGeneracion(Poblacion * pBase, int porc_torneo, int porc_elitismo, double prob_cruce, double prob_mutacion)
{
    Poblacion * nuevaGen = nuevaPoblacion(pBase->tamanio, pBase->genes_ind, 0);
    Individuo * padre1 = NULL;
    Individuo * padre2 = NULL;
    Individuo * hijo = NULL;

    // Elitismo: elegir una porción de los mejores individuos.
    int elitismo = (int) (((double) porc_elitismo * pBase->tamanio) / 100.0);
    int * elite = obtenerMejores(pBase, elitismo);

    int i;
    for(i = 0; i < elitismo; ++i)
        copiarIndividuo(nuevaGen->individuos[i], pBase->individuos[elite[i]]);

    for(i = elitismo; i < pBase->tamanio; ++i)
    {
        /** Selección por Torneo Determinista **/
        // padre1 = seleccionTorneoDeterminista(pBase, porc_torneo);
        // padre2 = seleccionTorneoDeterminista(pBase, porc_torneo);

        /** Selección por Torneo Probabilista **/
        // padre1 = seleccionTorneoProbabilista(pBase);
        // padre2 = seleccionTorneoProbabilista(pBase);

        /** Selección por Ruleta **/
        padre1 = seleccionRuleta(pBase);
        padre2 = seleccionRuleta(pBase);

        // Cruce uniforme
        hijo = cruzaUniforme(padre1, padre2, prob_cruce);
        copiarIndividuo(nuevaGen->individuos[i], hijo);

        destruirIndividuo(&padre1);
        destruirIndividuo(&padre2);
        destruirIndividuo(&hijo);
    }

    // Mutación
    mutarPoblacion(nuevaGen, prob_mutacion);
    calcularFitnessPoblacion(nuevaGen);

    free(elite);

    return nuevaGen;
}

int * obtenerMejores(Poblacion * poblacion, int n)
{
    double * aptitudes = (double *) malloc(poblacion->tamanio * sizeof(double));
    int * indices = (int *) malloc(poblacion->tamanio * sizeof(int));
    int * mejores = (int *) malloc(n * sizeof(int));

    int i, j;
    for(i = 0; i < poblacion->tamanio; ++i)
    {
        aptitudes[i] = (poblacion->individuos[i])->aptitud;
        indices[i] = i;
    }

    for(i = 0; i < n; ++i)
    {
        for(j = i + 1; j < poblacion->tamanio; ++j)
        {
            if(aptitudes[i] < aptitudes[j])
            {
                double tmp_apt = aptitudes[i];
                int tmp_pos = indices[i];

                aptitudes[i] = aptitudes[j];
                aptitudes[j] = tmp_apt;

                indices[i] = indices[j];
                indices[j] = tmp_pos;
            }
        }

        mejores[i] = indices[i];
    }

    free(aptitudes);
    free(indices);

    return mejores;
}

#endif // POBLACION_H_INCLUDED
