#ifndef MOTORPRINCIPAL_H
#define MOTORPRINCIPAL_H

#include "estructuras.h"

//Funciones Auxiliares
void combinarCartasJugador(List *manoJugador, Mesa mesa, Carta cartasCombinadas[]);
int contarJugadoresPendientes(CList *jugadores, Jugador *actual);
int obtenerApuestaMaxima(CList *jugadores);
void funcionTrampa(Partida *partida);

//Funciones gestion mesa
void crearFlop(Partida *partida);
void crearTurn(Partida *partida);
void crearRiver(Partida *partida);

//Funciones Principales de la partida
void moverIzquierdaBoton(Partida *partida);
void definirGanador(Partida *partida);
static Accion tomarDecisiones(Partida *partida, Jugador *jugadorActual, int apuestaMax);
void limpiarManos(Partida *partida);
void rondaDeApuestas(Partida *partida);
void iniciarRonda(Partida *partida);
void iniciarPartida();


#endif