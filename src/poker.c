#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/clist.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>

#define NOMINMAX         // Evita las macros min/max que chocan con las de C++
#define WIN32_LEAN_AND_MEAN // Reduce el tamaño de windows.h
#define NOGDI            // Evita la inclusión de funciones GDI, como la función Rectangle de Windows
#define NOUSER           // Evita la inclusión de funciones de usuario, como CloseWindow y ShowCursor de Windows
#include <windows.h>
//#include "raylib.h"


//Del poker
#include "tdasPoker/estructuras.h"
#include "tdasPoker/logicaCartas.h"
#include "tdasPoker/accionesJugador.h"
#include "tdasPoker/visualizacion.h"
#include "tdasPoker/motorPrincipal.h"
#include "tdasPoker/ventana.h"

// -----------------------------------------------------------------------

//posible cosa o no c xd, hola benjoid owo

//GrafoMano crearGrafoMano(List *manoJugador, Mesa mes);

/*
void definirGanadorPARTIDA(Partida partida) //incompleto
{
	Jugador *jug = clist_first(partida.jugadores);
	Jugador *inicio = jug;
	int mayorfichas = jug->fichas;
	Jugador *best = jug;
	do{
		if ( jug->fichas > mayorfichas )
		{
			mayorfichas = jug->fichas;
			best = jug;
		}
		jug = clist_next(jugadores);
	} while ( jug != inicio );
	
	printf("¡%c es el ganador definitivo!", best->nombre )
}
*/

int main(){
	SetConsoleOutputCP(CP_UTF8); //gracias compañero de telegram

	limpiarPantalla();
	//intro(300));
	limpiarPantalla();
	int IArand;

	char opcion;
	do{
		puts("========================================");
		puts("           SSADASD♠️  ♥️  Poker  ♦️  ♣️");
		puts("========================================");
		printf("| \033[5;90m7 ♠ \033[0m| \033[1;31m 7 ♥ \033[0m| \033[1;32m ♣ 7 \033[0m| \033[1;34m 7 ♦\033[0m |\n");
		puts("1) Iniciar Partida");
		puts("2) Salir");
		puts("3) Activar IArand");

		printf("Ingrese su opción: ");
		scanf(" %c", &opcion);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);

		switch (opcion){
		case '1':
			//iniciarVentana();
			iniciarPartida(IArand);
			
			break;
		case '3':
			//ahora es un toggle, será usado para que la IA no haga siempre las mejores decisiones. 
			if ( IArand = 0 )
			{
				IArand = 1;
			} else
			{
				IArand = 0;
			}
			break;
		default:
			printf("Opción no válida\n");
			break;
		}
		presioneTeclaParaContinuar();
		limpiarPantalla();

	} while (opcion != '2');

	return 0;
}
