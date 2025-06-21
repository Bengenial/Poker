#ifndef VISUALIZACION_H
#define VISUALIZACION_H

#include <stdio.h>

#include "../tdas/list.h"
#include "../tdas/clist.h"
#include "../tdas/extra.h"

#include <time.h>

#define NOMINMAX         // Evita las macros min/max que chocan con las de C++
#define WIN32_LEAN_AND_MEAN // Reduce el tamaño de windows.h
#define NOGDI            // Evita la inclusión de funciones GDI, como la función Rectangle de Windows
#define NOUSER           // Evita la inclusión de funciones de usuario, como CloseWindow y ShowCursor de Windows
#include <windows.h>
#include "raylib.h"

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