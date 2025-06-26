#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/clist.h"
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


int main(){
	SetConsoleOutputCP(CP_UTF8); //gracias compañero de telegram

	limpiarPantalla();
	//intro(300));
	limpiarPantalla();

	char opcion;
	do{
		puts("=========================================");
		puts("| \033[5;90m ♠  \033[1;31m ♥  \033[1;32m ♣  \033[1;34m ♦  \033[0mPOKER\033[1;34m  ♦ \033[1;32m  ♣ \033[1;31m  ♥  \033[5;90m ♠ \033[0m |");
		puts("=========================================");
		//printf("| \033[5;90m7 ♠ \033[0m| \033[1;31m 7 ♥ \033[0m| \033[1;32m ♣ 7 \033[0m| \033[1;34m 7 ♦\033[0m |\n");
		puts("1) Iniciar Partida");
		puts("2) Salir");
		//puts("3) Activar IArand");

		printf("Ingrese su opción: ");
		scanf(" %c", &opcion);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);

		switch (opcion){
		case '1':
			//iniciarVentana();
			limpiarPantalla();
			printf("=================\n");
			printf("  Configuración\n");
			printf("=================\n");
			iniciarPartida();
			break;
			
		case '2':
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
