#ifndef ACCIONESJUGADOR_H
#define ACCIONESJUGADOR_H

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
#include "logicaCartas.h"
#include "visualizacion.h"
#include "motorPrincipal.h"

//Gestion de jugadores
Jugador *crearJugador(char *nombre, int esBot);
void eliminarJugadores(CList *jugadores);
int contarJugadoresActivos(CList *jugadores, Jugador *actual);

//Acciones en rondaApuestas
void checkOrCall(Jugador *jugadorActual, int apuestaActual, Partida *partida, int *jugadoresPendientes);
void raise(Jugador *actual, int *apuestaMax, Partida *partida, int *jugadoresPendientes, Jugador *inicio, int  *cantidad, Jugador *jug);
void fold(Jugador *actual, int *jugadoresPendientes, Partida *partida, int *salir);

#endif