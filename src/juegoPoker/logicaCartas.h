#ifndef LOGICACARTAS_H
#define LOGICACARTAS_H

#include "estructuras.h"

//Funciones de creacion de cartas
void intercambiarCartas(Carta *a, Carta *b);    //Un swap para cartas

void leerCartas(Baraja *baraja);    //Lee las cartas desde un archivo CSV

void barajarCartas(Baraja *baraja);     //baraja las cartas de la baraja

void repartirCartas(Partida *partida);      //Reparte las cartas a los jugadores de la partida

//Logica de evaluacion
int compararCartas(const void *a, const void *b);   //Compara dos cartas por su valor

void combinarCartasJugador(List *manoJugador, Mesa mesa, Carta cartasCombinadas[]);     //Combina las cartas del jugador con las de la mesa para formar una mano completa

ManoEvaluada evaluarMano(Carta cartas[], int numCartas);    //Evalua la mano de un jugador basándose en las cartas dadas

//Funciones auxiliares de apoyo
int obtenerValorCarta(char *valor);     //Obtiene el valor de una carta dada su representación en cadena

int obtenerIndicePalo(char *color);     //Obtiene el índice del palo de una carta dada su representación en cadena

int verificarColor(Carta cartas [], int numCartas, int *valoresColor);      //Verifica si la mano tiene color o no

int verificarEscalera(Carta cartas[], int numCartas, int *valorEscalera);   //Verifica si la mano tiene escalera o no

void contarRepeticiones(Carta cartas[], int numCartas, int repeticiones[], int valores[]);      //Cuenta las repeticiones de cada valor en las cartas dadas


#endif