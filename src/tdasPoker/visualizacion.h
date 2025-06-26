#ifndef VISUALIZACION_H
#define VISUALIZACION_H

#include <stdio.h>

#include "../tdas/list.h"
#include "../tdas/clist.h"
#include "../tdas/extra.h"
#include <time.h>
#include <windows.h>

//Del poker
#include "estructuras.h"
#include "logicaCartas.h"
#include "accionesJugador.h"
#include "motorPrincipal.h"

void intro(int timeset);
void mostrarMesa(Mesa mesa);
void mostrarCartasJugador(Jugador *jugador);
void mostrarMano(List *mano);
void mostrarCarta(Carta carta);
void mostrarTipoMano(TipoMano tipo);
void mostrarGandorFold(Partida *partida);


#endif