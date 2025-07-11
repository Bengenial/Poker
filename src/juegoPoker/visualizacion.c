#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//TDAS
#include "../tdas/list.h"
#include "../tdas/clist.h"
#include "../tdas/extra.h"

//JUEGOPOKER
#include "estructuras.h"
#include "accionesJugador.h"

void arcoiris(char *texto){
    // Colores ANSI para simular arcoíris
    int colores[] = {31, 33, 32, 36, 34, 35}; // rojo, amarillo, verde, cyan, azul, magenta
    int num_colores = sizeof(colores) / sizeof(colores[0]);

    for (int i = 0; i < (int) strlen(texto); i++) {
        int color = colores[i % num_colores];
        printf("\033[1;%dm%c", color, texto[i]); 
    }

    printf("\033[0m"); // Reset al final
}

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
	Sleep(3000);
    printf("\n\n");
	limpiarPantalla();
}

void mostrarCarta(Carta carta)
{
	if(strcmp(carta.color, "corazones") == 0) // son iguales
	{ 
		printf("\033[1;91m%s ♥\033[0m", carta.valor);
		return;
	}

	if(strcmp(carta.color, "diamantes") == 0)
	{ 
		printf("\033[1;94m%s ♦\033[0m", carta.valor);
		return;
	}
	
	if(strcmp(carta.color, "picas") == 0)
	{ 
		printf("\033[1;90m%s ♠\033[0m", carta.valor);
		return;
	}
	
	printf("\033[1;92m%s ♣\033[0m", carta.valor);
	
}

void mostrarMesa(Partida *partida)
{	
	printf("MESA ACTUAL:\n");
	printf("BOTE: %d\n\n", partida->mesa.bote);
	printf(".---------------------.\n");
	if(partida->mesa.total == 0){
	printf("|\033[5;92mPRE-FLOP\033[0m             |\n");
	printf("|CIEGA MAYOR: %d      |\n", partida->mesa.ciegaMayor);
	} 


	switch (partida->mesa.total)
	{
	case 3:
		printf("|\033[5;93m        FLOP\033[0m         |\n");
		break;
	case 4:
		printf("|\033[5;38;5;208m        TURN\033[0m         |\n");
		break;
	case 5:
		printf("|\033[5;91m        RIVER\033[0m        |\n");
		break;
	}
	printf(".---------------------.\n");

	for(int k = 0 ; k < partida->mesa.total; k++)
	{
		printf("Carta [%d]: ", k+1);
		mostrarCarta(partida->mesa.cartas[k]);
		printf("\n");
	}
	
}

void mostrarCartasJugador(Jugador *jugador) {
    printf("%s: ", jugador->nombre);
    Carta *carta = list_first(jugador->mano);
    while (carta != NULL) {
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

void mostrarGanadorFinal(Partida *partida) {
	limpiarPantalla();
	printf("=== RESULTADO FINAL ===\n\n");
	Sleep(3000);
	
	
	printf("Ganador de la partida: %s\n", partida->ganador->nombre);
	printf("Fichas finales: %d\n", partida->ganador->fichas);
	
	Sleep(3000);
	printf("\n\n");
	arcoiris("GRACIAS POR JUGAR\n");
}

void mostrarSalida(Partida *partida){
	limpiarPantalla();
	printf("=== PARTIDA TERMINADA ===\n\n");
	Sleep(3000);

	Jugador *jug = clist_first(partida->jugadores);
	while (jug != NULL) {
		if (!jug->esBot) {
			break;
		}
		jug = clist_next(partida->jugadores);
	}
	if (jug->fichas == 0){
		printf("Lo sentimos %s\n", jug->nombre);
		printf("Has perdido todas tus fichas, ¡mejor suerte la próxima vez!\n");
	} else if(jug->fichas > 0 && jug->fichas < 100){
		printf("Una salida estratégica %s\n", jug->nombre);
		printf("Has terminado con %d fichas, ¡sigue practicando!\n", jug->fichas);
	} else if (jug->fichas == 100) {
		printf("Bueno, %s\n", jug->nombre);
		printf("Saliste con las mismas fichas que cuando entraste\n");
	}
	else {
		printf("Buen juego %s\n", jug->nombre);
		printf("Te has llevado %d fichas\n", jug->fichas);
	}
	
	
	Sleep(3000);
	printf("\n\n");
	arcoiris("GRACIAS POR JUGAR\n");
}