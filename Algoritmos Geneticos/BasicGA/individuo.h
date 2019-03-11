#ifndef INDIVIDUO_H_INCLUDED
#define INDIVIDUO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int * genes;
    int longitud;
    double aptitud;
} Individuo;

/** Prototipos **/

// Operaciones básicas.
Individuo * nuevoIndividuo(int, int);
void copiarIndividuo(Individuo *, Individuo *);
void destruirIndividuo(Individuo **);
void imprimirIndividuo(Individuo *);
int aleatorio(int);

// Operaciones AG.
void calcularFitnessIndividuo(Individuo *, int *);
void mutarIndividuo(Individuo *, double);
Individuo * cruzaUniforme(Individuo *, Individuo *, double);

/** Implementación **/

Individuo * nuevoIndividuo(int longitud, int inicializar)
{
    Individuo * nuevo = (Individuo *) malloc(sizeof(Individuo));
    nuevo->genes = (int *) malloc(longitud * sizeof(int));
    nuevo->longitud = longitud;
    nuevo->aptitud = 0.0;

    if(inicializar)
    {
        int i;
        for(i = 0; i < longitud; ++i)
            nuevo->genes[i] = aleatorio(10);
    }

    return nuevo;
}

void copiarIndividuo(Individuo * destino, Individuo * fuente)
{
    if(destino && fuente)
    {
        if(destino->longitud == fuente->longitud)
        {
            destino->aptitud = fuente->aptitud;

            int i;
            for(i = 0; i < destino->longitud; ++i)
                destino->genes[i] = fuente->genes[i];
        } else
            printf("Los individuos tienen longitudes distintas: %d != %d", destino->longitud, fuente->longitud);
    }
}

void destruirIndividuo(Individuo ** individuo)
{
    if(individuo)
    {
        if(*individuo)
        {
            if((*individuo)->genes)
            {
                free((*individuo)->genes);
                (*individuo)->genes = NULL;
            }

            free(*individuo);
            *individuo = NULL;
        }
    }
}

void imprimirIndividuo(Individuo * individuo)
{
    if(individuo)
    {
        printf("Individuo: ");

        int i;
        for(i = 0; i < individuo->longitud; ++i)
            printf("%d ", individuo->genes[i]);

        printf("Fitness: %g\n", individuo->aptitud);
    }
}

int aleatorio(int lim)
{
    return rand() % lim;
}

void calcularFitnessIndividuo(Individuo * individuo, int * objetivo)
{
    if(individuo)
    {
        individuo->aptitud = 0.0;

        int i;
        for(i = 0; i < individuo->longitud; ++i)
        {
            if(individuo->genes[i] == objetivo[i])
                individuo->aptitud += 1.0;
        }

        individuo->aptitud /= individuo->longitud;
    }
}

void mutarIndividuo(Individuo * individuo, double prob_mutacion)
{
    if(individuo)
    {
        double rnd;
        int i;

        for(i = 0; i < individuo->longitud; ++i)
        {
            rnd = (double) rand() / (double) ((unsigned)RAND_MAX + 1);
            if(rnd <= prob_mutacion)
                individuo->genes[i] = aleatorio(10);
        }
    }
}

Individuo * cruzaUniforme(Individuo * padre1, Individuo * padre2, double prob_cruce)
{
    Individuo * hijo = nuevoIndividuo(padre1->longitud, 0);

    double rnd;
    int i;

    for(i = 0; i < hijo->longitud; ++i)
    {
        rnd = (double) rand() / (double) ((unsigned)RAND_MAX + 1);
        if(rnd <= prob_cruce)
            hijo->genes[i] = padre1->genes[i];
        else
            hijo->genes[i] = padre2->genes[i];
    }

    return hijo;
}

#endif // INDIVIDUO_H_INCLUDED
