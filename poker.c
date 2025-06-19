#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/clist.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>
#include <windows.h>

//Del poker
#include "tdasPoker/estructuras.h"
#include "tdasPoker/logicaCartas.h"
#include "tdasPoker/accionesJugador.h"
#include "tdasPoker/visualizacion.h"
#include "tdasPoker/motorPrincipal.h"
//#include "tdasPoker/funcionesnormal.h"
// id

// -----------------------------------------------------------------------

/*Accion tomarDecisiones(ManoEvaluada manoActual, int apuestaActual)
{	
	//Tener en cuenta que la IA es lo más basico, osea va a ser predesible (por el momento...)
	
	if(mano.puntuacion < 1500000) //numero por definirse, pero es un ejemplo
	{ //pesima mano
	 	if(apuestaActual > 0) return ACCION_FOLD; //en este caso si aumentan la apuesta, me retiro (fold)
		else return ACCION_CHECK; si no aumentan la apuesta, chekeamos (seguimos)
	}

	else{
		if(mano.puntuacion < 30000000)
		{ //mano decente
		 if(apuestaActual > 0) return ACCION_CALL; //ya que tiene una mano decente, puede permitirse igualar la apuesta
		 else return ACCION_CHECK;
		}
		else{ //si tiene una muy buena mano
			return ACCION_RAISE; va a aumentar la apuesta
		}
	}
}*/

/*	//Esto va dentro de la ronda de apuestas, en caso de que sea bot y en que sea humano
    //Por mientras lo dejo comentado mientras benjoid cocina
typedef struct{
	char nombre[50];
	int fichas;
	List *mano;
	int esBot;
	char estado[10]; //jugando, retirado, ganador
	int apuesta;
	int yaActuo;
}Jugador;

	Jugador *jugadorActual = partida->jugadores->actual;

	do{
		if(strcmp(jugadorActual->estado, "jugando") == 0 ) continue;// está jugando

		if(jugadorActual->esBot){ // 1 si e sbot, osea, verdadero
		Accion accionATomar = tomarDeciciones(jugadorActual->mano, jugadorActual->apuesta);
			switch(accionATomar){
				case ACCION_FOLD:
					strcpy(jugadorActual->estado, "Retirado");
					printf("%s se retira.\n", jugadorActual->nombre);
					break;
				case ACCION_CHECK:
					printf("%s pasa.\n", jugadorActual->nombre);
					break;
				case ACCION_CALL:
					//igualar apuesta
					printf("%s iguala la apuesta.\n", jugadorActual->nombre);
					break;
				case ACCION_RAISE:
					//aumentar apuesta (que sea pequeña, es de prueba)
					printf("%s aumenta la apuesta.\n", jugadorActual->nombre);
					break;
			}
		} else {
			//jugador humano, pedirle que ingrese una accion
			//mostrar mano, mesa, fichas, apuestas, etc.
			//pedirle que ingrese una accion
		}
	}

*/

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
		puts("           ♠️  ♥️  Poker  ♦️  ♣️");
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
