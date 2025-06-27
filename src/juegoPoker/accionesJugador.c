#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TDAS
#include "../tdas/extra.h"

//JUEGOPOKER
#include "accionesJugador.h" 
#include "visualizacion.h"
#include "motorPrincipal.h"

//Gestion de jugadores
Jugador *crearJugador(char *nombre, int esBot){
	Jugador *jugador = malloc(sizeof(Jugador));
	if(!jugador){
			printf("ERROR al asignarle memoria al jugador");
			EXIT_FAILURE;
	}
	strcpy(jugador->nombre, nombre);
	strcpy(jugador->estado, "Jugando");
	jugador->fichas = 100;
	jugador->mano = list_create();
	jugador->apuesta = 0;
	jugador->esBot = esBot;
	jugador->hizoRiseCall = 0;
	return jugador;
}

void eliminarJugadores(CList *jugadores)
{
	Jugador *jug = clist_first(jugadores);
	Jugador *inicio = jug;
	do{
		if ( jug->fichas == 0 )
		{
			strcpy(jug->estado, "Eliminado");
		}
		jug = clist_next(jugadores);
	} while ( jug != inicio );
}
int jugadoresJugando(CList *jugadores, Jugador *actual) {
	int n = 0;
	Jugador *jug = clist_first(jugadores);
	if (!jug) return 0;
	Jugador *inicio = jug;
	do {
		if (strcmp(jug->estado, "Jugando") == 0) n++;
		jug = clist_next(jugadores);
	} while (jug != inicio);

	if (n > 1){
		do{
			if(jug == actual) break;
			jug = clist_next(jugadores);
		}while (jug != actual);
	}

	return n;
}

int contarJugadoresActivos(CList *jugadores, Jugador *actual) {

    int n = 0;
    Jugador *jug = clist_first(jugadores);
    if (!jug) return 0;
    Jugador *inicio = jug;
    do {
        if (strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0)
		{
			n++;
		}
        jug = clist_next(jugadores);
    } while (jug != inicio);
	
	// devolver a su actual
	if (n > 1){
		do{
			if(jug == actual) break;
			jug = clist_next(jugadores);
		}while (jug != actual);
	}
	
    return n;
}

int contarJugadoresPendientes(CList *jugadores, Jugador *actual) {
    int n = 0;
    Jugador *jug = clist_first(jugadores);
    if (!jug) return 0;
    Jugador *inicio = jug;
    do {
        if (strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0 && !jug->yaActuo) n++;
        jug = clist_next(jugadores);
    } while (jug != inicio);


	//devolver actual (current)
	do{
		if(jug == actual) break;
		jug = clist_next(jugadores);
	}while (jug != actual);
	
    return n;
}

//Acciones en rondaApuestas
void checkOrCall(Jugador *jugadorActual, int apuestaActual, Partida *partida, int *jugadoresPendientes){
	
	int paraIgualar = apuestaActual - jugadorActual->apuesta;

	if(paraIgualar == 0)
	{
		printf("%s \033[1;92mpasa\033[0m\n", jugadorActual->nombre);
		jugadorActual->yaActuo = 1;
		(*jugadoresPendientes)--;
		return;
	}
	
	int cantidadApostar;

	//Si no tiene para igualar
	if(paraIgualar >= jugadorActual->fichas)//o el para igualar es mayor a la fichas del jugador
	{
		cantidadApostar = jugadorActual->fichas;
	}
	else{
		cantidadApostar = paraIgualar;//si no, pues la cantidad a apostar será la para igualar
	}

	//actualizamos los estados
	jugadorActual->fichas -= cantidadApostar;
    jugadorActual->apuesta += cantidadApostar;
    partida->mesa.bote += cantidadApostar;

	printf("%s \033[1;93miguala la apuesta\033[0m\n", jugadorActual->nombre);

	if(jugadorActual->fichas == 0) //si quedo pobre
	{
		printf("%s va ", jugadorActual->nombre);
		arcoiris("all-in");
		puts("");
	}

	jugadorActual->yaActuo = 1;
	jugadorActual->hizoRiseCall = 1;
	(*jugadoresPendientes)--;

	//presioneTeclaParaContinuar();

	/*if (jugadorActual->apuesta == apuestaActual) {
		printf("%s pasa.\n", jugadorActual->nombre);
	} else {
		int diferencia = apuestaActual - jugadorActual->apuesta;
		if (jugadorActual->fichas >= diferencia) {
			jugadorActual->fichas -= diferencia;
			jugadorActual->apuesta += diferencia;
			partida->mesa.bote += diferencia;
			printf("%s \033[1;93miguala la apuesta.\033[0m\n", jugadorActual->nombre);
			if (jugadorActual->fichas == 0){
				printf("%s va ", jugadorActual->nombre);
				arcoiris("all-in");
				puts("");
			} 
		} else {
			partida->mesa.bote += jugadorActual->fichas;
			jugadorActual->apuesta += jugadorActual->fichas;
			jugadorActual->fichas = 0;
			printf("%s va ", jugadorActual->nombre);
			arcoiris("all-in");
			puts("");
		}
	}
	jugadorActual->yaActuo = 1;
	jugadorActual->hizoRiseCall = 1;
	(*jugadoresPendientes)--;*/
}

void raise(Jugador *actual, int *apuestaMax, Partida *partida, int *jugadoresPendientes, Jugador *inicio, int  *cantidad, Jugador *jug){
	if(!actual->esBot){
		int minimoApuesta = ((*apuestaMax) - actual->apuesta) + 1;
		
		do{
			printf("¿Cuánto quieres subir? (mínimo %d): ", minimoApuesta);
			scanf("%d", cantidad);
			getchar();
			
			if((*cantidad) < minimoApuesta && (*cantidad) < actual->fichas)
				printf("El Raise que hiciste es más bajo que el minimo u_u, \nIntentalo nuevamente :D\n");


		}while ((*cantidad) < minimoApuesta &&  (*cantidad) < actual->fichas);
	}
	else{
		//Por ahora solo va a subir 10
		int cantidadAAnadir = (*apuestaMax + 10) - actual->apuesta;
		(*cantidad) = cantidadAAnadir;
	}

	if ((*cantidad) > actual->fichas) (*cantidad) = actual->fichas;

	actual->fichas -= (*cantidad);
	actual->apuesta += (*cantidad);
	partida->mesa.bote += (*cantidad);
	(*apuestaMax) = actual->apuesta;
	actual->hizoRiseCall = 1;


	printf("%s \033[1;96msube la apuesta\033[0m a %d.\n", actual->nombre, actual->apuesta);
	//nuevo de allin
	if(actual->fichas == 0)
	{
		printf("%s va ",actual->nombre);
		arcoiris("all-in");
		puts("");

	}


	// Reabrir ronda: todos deben responder al nuevo raise excepto quien lo hizo y los retirados
	jug = clist_first(partida->jugadores);
	Jugador *inicio2 = jug;
	do {
		if (jug != actual && strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0)
			jug->yaActuo = 0;
		jug = clist_next(partida->jugadores);
	} while (jug != inicio2);
	actual->yaActuo = 1;
	(*jugadoresPendientes) = contarJugadoresPendientes(partida->jugadores, actual);
	
	inicio = actual;
	//presioneTeclaParaContinuar();
}

void fold(Jugador *actual, int *jugadoresPendientes, Partida *partida, int *salir){
	strcpy(actual->estado, "Retirado");
	printf("%s \033[1;95mse retira.\033[0m\n", actual->nombre);
	actual->yaActuo = 1;

	//solo restar si hay más de un jugador
	(*jugadoresPendientes)--;
	// Si solo queda uno, termina la ronda
	if (contarJugadoresActivos(partida->jugadores, actual) == 1){
		Jugador *actual = clist_first(partida->jugadores);
		Jugador *inicio = actual;
		do {
			if (strcmp(actual->estado, "Jugando") == 0){
				partida->ganador = actual;
				break;
			}
			actual = clist_next(partida->jugadores);
		} while (actual != inicio);
		
		(*salir) = 0;
		(*jugadoresPendientes) = 0; //no crear un bucle infinito
	}
	//presioneTeclaParaContinuar();
}
