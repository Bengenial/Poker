#ifndef LOGICACARTAS_H
#define LOGICACARTAS_H

#include "estructuras.h"

//Funcion barajas en si
void intercambiarCartas(Carta *a, Carta *b);
void leerCartas(Baraja *baraja);
void barajarCartas(Baraja *baraja);
void repartirCartas(Partida *partida);

//Logica de evaluacion
int compararCartas(const void *a, const void *b);
ManoEvaluada evaluarMano(Carta cartas[], int numCartas);

//Funciones auxiliares de apoyo
int obtenerValorCarta(char *valor);
int obtenerIndicePalo(char *color);
int verificarColor(Carta cartas [], int numCartas, int *valoresColor);
int verificarEscalera(Carta cartas[], int numCartas, int *valorEscalera);
void contarRepeticiones(Carta cartas[], int numCartas, int repeticiones[], int valores[]);


#endif