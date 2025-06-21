#ifndef ACCIONESJUGADOR_H
#define ACCIONESJUGADOR_H

//Del poker
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