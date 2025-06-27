#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

//TDAS
#include "tdas/list.h"
#include "tdas/clist.h"
#include "tdas/extra.h"


//JUEGOPOKER
#include "juegoPoker/estructuras.h"
#include "juegoPoker/logicaCartas.h"
#include "juegoPoker/accionesJugador.h"
#include "juegoPoker/visualizacion.h"
#include "juegoPoker/motorPrincipal.h"



int main(){
	SetConsoleOutputCP(CP_UTF8); //gracias compañero de telegram

	limpiarPantalla();
	//intro(300));
	int IArand;

	char opcion;
	do{
		puts("=========================================");
		puts("|\033[5;90m  ♠\033[5;91m   ♥\033[5;92m   ♣\033[5;94m   ♦\033[0m  POKER\033[5;94m  ♦\033[5;92m   ♣\033[5;91m   ♥\033[5;90m   ♠  \033[0m|");
		puts("=========================================");
		puts("1) Iniciar Partida");
		puts("2) Salir");

		printf("Ingrese su opción: ");
		scanf(" %c", &opcion);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);

		switch (opcion){
		case '1':
			//iniciarVentana();
			iniciarPartida();
			return 0; // Termina el programa después de terminar la partida

			break;
		case '2':
			//ahora es un toggle, será usado para que la IA no haga siempre las mejores decisiones. 
			printf("Saliendo del juego...\n");
			
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
