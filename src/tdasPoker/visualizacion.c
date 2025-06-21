#include <stdio.h>
#include <stdlib.h>
#include "../tdas/list.h"
#include "../tdas/clist.h"

#define NOMINMAX         // Evita las macros min/max que chocan con las de C++
#define WIN32_LEAN_AND_MEAN // Reduce el tamaño de windows.h
#define NOGDI            // Evita la inclusión de funciones GDI, como la función Rectangle de Windows
#define NOUSER           // Evita la inclusión de funciones de usuario, como CloseWindow y ShowCursor de Windows
#include <windows.h>
#include "raylib.h"

//Del poker
#include "estructuras.h"
#include "accionesJugador.h"


void intro(int timeset){
	limpiarPantalla();
    printf("\n");

    printf("  ____   ___   _  __ _____ ____  \n");
	Sleep(timeset);
    //Sleep(timeset); // 300 ms, funcion de <windows.h>

    printf(" |  _ \\ / _ \\ | |/ /| ____|  _ \\ \n");
    //Sleep(timeset);
	Sleep(timeset);

    printf(" | |_) | | | || ' / |  _| | |_) |\n");
    //Sleep(timeset);
	Sleep(timeset);

    printf(" |  __/| |_| || . \\ | |___|  _ < \n");
    //Sleep(timeset);
	Sleep(timeset);

    printf(" |_|    \\___/ |_|\\_\\|_____|_| \\_\\\n");
    //Sleep(timeset*3); //300
	Sleep(timeset * 3);
    printf("\n\n");
}

void mostrarMesa(Mesa mesa)
{	
	printf("MESA ACTUAL:\n");
	printf("BOTE: %d\n\n", mesa.bote);
	switch (mesa.total)
	{
	case 3:
		printf("FLOP\n");
		break;
	case 4:
		printf("TURN\n");
		break;
	case 5:
		printf("RIVER\n");
		break;
	}
	
	for(int k = 0 ; k < mesa.total; k++)
	{
		printf("Carta [%d]: ", k+1);//, mesa.cartas[k].valor);
		mostrarCarta(mesa.cartas[k]);
		printf("\n");
	}
	
}

void mostrarCartasJugador(Jugador *jugador) {
    printf("%s: ", jugador->nombre);
    Carta *carta = list_first(jugador->mano);
    while (carta != NULL) {
        //printf("%s", carta->valor);
		mostrarCarta(*carta);
		printf("   ");
        carta = list_next(jugador->mano);
    }
}

void mostrarMano(List *mano){
	Carta* carta = list_first(mano);

	printf("Tus cartas:\n");
	while(carta){
		//printf("%s", carta->valor);
		mostrarCarta(*carta);
		printf("\n");
		carta = list_next(mano);
	}
}

void mostrarCarta(Carta carta)
{
	if(strcmp(carta.color, "corazones") == 0) // son iguales
	{ 
		printf("\033[1;31m%s ♥\033[0m", carta.valor);
		return;
	}

	if(strcmp(carta.color, "diamantes") == 0)
	{ 
		printf("\033[1;34m%s ♦\033[0m", carta.valor);
		return;
	}
	
	if(strcmp(carta.color, "picas") == 0)
	{ 
		printf("\033[5;90m%s ♠\033[0m", carta.valor);
		return;
	}
	
	printf("\033[1;32m%s ♣\033[0m", carta.valor);
	
}

void mostrarTipoMano(TipoMano tipo) {
    switch (tipo) {
        case ESCALERA_REAL:
            printf("Escalera Real");
            break;
        case ESCALERA_COLOR:
            printf("Escalera de Color");
            break;
        case POKER_MANO:
            printf("Poker");
            break;
        case FULL_HOUSE:
            printf("Full House");
            break;
        case COLOR:
            printf("Color");
            break;
        case ESCALERA:
            printf("Escalera");
            break;
        case TRIO:
            printf("Trio");
            break;
        case DOS_PARES:
            printf("Dos Pares");
            break;
        case PAR:
            printf("Par");
            break;
        case CARTA_ALTA:
            printf("Carta Alta");
            break;
    }
}

void mostrarGandorFold(Partida *partida){
	printf("TODOS LOS JUGADORES SE HAN RETIRADO\n");
	printf("EL JUGADOR %s ha ganado %d fichas\n", partida->ganador->nombre, partida->mesa.bote);
	//repartir bote
	partida->ganador->fichas += partida->mesa.bote;
	partida->mesa.bote = 0;

}