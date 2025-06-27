#ifndef VISUALIZACION_H
#define VISUALIZACION_H

#include "estructuras.h"

void arcoiris(char *texto);   //Muestra un texto con colores simulando un arcoíris, útil para mensajes destacados

void intro(int timeset);    //Una introducción al juego, mostrando un mensaje de bienvenida y animaciones iniciales

void mostrarMesa(Partida *partida);    //Muestra las cartas de la mesa, incluyendo el flop, turn y river

void mostrarCartasJugador(Jugador *jugador);    //Muestra las cartas de un jugador, incluyendo su mano y el estado de la partida (definir ganador)

void mostrarMano(List *mano);       //Muestra las cartas de la mano de un jugador, iterando sobre su lista de cartas (ronda de apuestas)

void mostrarCarta(Carta carta);   //Muestra una carta individual, con su valor y color

void mostrarTipoMano(TipoMano tipo);    //Muestra el tipo de mano que tiene un jugador, como par, trío, escalera, etc.

void mostrarGandorFold(Partida *partida);   //Muestra el ganador de la partida o si un jugador ha hecho fold, actualizando el estado de la partida

void mostrarGanadorFinal(Partida *partida);   //Muestra el ganador final de la partida

#endif