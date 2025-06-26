#ifndef VISUALIZACION_H
#define VISUALIZACION_H

#include "estructuras.h"

void intro(int timeset);
void mostrarMesa(Mesa mesa);
void mostrarCartasJugador(Jugador *jugador);
void mostrarMano(List *mano);
void mostrarCarta(Carta carta);
void mostrarTipoMano(TipoMano tipo);
void mostrarGandorFold(Partida *partida);


#endif