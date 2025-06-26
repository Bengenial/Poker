#ifndef MOTORPRINCIPAL_H
#define MOTORPRINCIPAL_H

#include <stdio.h>
#include <stdlib.h>
#include "../tdas/list.h"
#include "../tdas/clist.h"
#include "../tdas/extra.h"
#include <string.h>
#include <time.h>
#include <windows.h>

//Del poker
#include "estructuras.h"
#include "logicaCartas.h"
#include "accionesJugador.h"
#include "visualizacion.h"

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