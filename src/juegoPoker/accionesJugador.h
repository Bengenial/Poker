#ifndef ACCIONESJUGADOR_H
#define ACCIONESJUGADOR_H

#include "estructuras.h"



//Gestion de jugadores
Jugador *crearJugador(char *nombre, int esBot); //Crea un nuevo jugador

void eliminarJugadores(CList *jugadores); //Elimina los jugadores sin fichas

int contarJugadoresActivos(CList *jugadores, Jugador *actual); //Cuenta los jugadores activos (con fichas y no retirados) y devuelve la cantidad

//Acciones en rondaApuestas
void checkOrCall(Jugador *jugadorActual, 
    int apuestaActual, Partida *partida, int *jugadoresPendientes); //Chequea o iguala la apuesta actual del jugador

void raise(Jugador *actual, int *apuestaMax, Partida *partida,
    int *jugadoresPendientes, Jugador *inicio, int  *cantidad, Jugador *jug); //Aumenta la apuesta del jugador actual

void fold(Jugador *actual, int *jugadoresPendientes,
    Partida *partida, int *salir); //Retira al jugador actual de la ronda

#endif