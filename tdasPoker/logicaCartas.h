#ifndef LOGICACARTAS_H
#define LOGICACARTAS_H

#include <stdio.h>
#include <stdlib.h>
#include "../tdas/list.h"
#include "../tdas/clist.h"
#include "../tdas/heap.h"
#include "../tdas/extra.h"
#include <string.h>
#include <time.h>
#include <windows.h>

//Del poker
#include "estructuras.h"
#include "accionesJugador.h"
#include "visualizacion.h"
#include "motorPrincipal.h"

//Funcion barajas en si
void intercambiarCartas(Carta *a, Carta *b);
void leerCartas(Baraja *baraja);
void barajarCartas(Baraja *baraja);
void repartirCartas(Partida *partida);

//Logica de evaluacion
int compararCartas(const void *a, const void *b);
ManoEvaluada evaluarMano(Carta cartas[], int numCartas);

//Funciones auxiliares de apollo
int obtenerValorCarta(char *valor);
int obtenerIndicePalo(char *color);
int verificarColor(Carta cartas [], int numCartas, int *valoresColor);
int verificarEscalera(Carta cartas[], int numCartas, int *valorEscalera);
void contarRepeticiones(Carta cartas[], int numCartas, int repeticiones[], int valores[]);


#endif