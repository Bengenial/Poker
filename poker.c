#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>
#include <windows.h>

typedef struct{
	int id;
	char color[10]; //corazones, diamantes, tréboles, picas
	char valor[3]; //2-10, J, Q, K, A
}Carta;

typedef struct{
	Carta cartas[52]; // ver si es factible hacerlo con un array o una lista
	int num_cartas;
}Baraja;

typedef struct{
	char nombre[50];
	int fichas;
	List *mano;
	char estado[10]; //jugando, retirado, ganador
}Jugador;

typedef struct{
	Carta cartas[5];
	int bote;
}Mesa;

typedef struct{
	List *jugadores;
	int numJugadores;
	Jugador *jugadorActual;
	Baraja baraja;
	Mesa mesa;
	int ronda;
}Partida;

void leerCartas(Baraja *baraja){
	FILE *archivo = fopen("data/cartas_poker.csv", "r");
	if (archivo == NULL){
		perror(
			"Error al abrir el archivo"); // Informa si el archivo no puede abrirse
		return;
	}
	char **campos;
	int i = 0;

	campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

	while ((campos = leer_linea_csv(archivo, ',')) != NULL){
		Carta carta;
		carta.id = atoi(campos[0]);
		strcpy(carta.valor, campos[1]);
		strcpy(carta.color, campos[2]);

		baraja->cartas[i] = carta;
		i++;
	}
	baraja->num_cartas = i;
	fclose(archivo); // Cierra el archivo después de leer todas las líneas
}

void intercambiarCartas(Carta *a, Carta *b){
	Carta aux = *a;
    *a = *b;
    *b = aux;
}

void barajarCartas(Baraja *baraja){
	static int semilla_inicializada = 0;
    if (!semilla_inicializada) {
        srand(time(NULL));
        semilla_inicializada = 1;
    }

    for (int i = baraja->num_cartas - 1; i > 0; i--) {
        int j = rand() % (i + 1); // índice aleatorio entre 0 e i
        intercambiarCartas(&baraja->cartas[i], &baraja->cartas[j]);
    }
}

void mostrarCartas(Baraja *baraja){
	for (int i = 0; i < baraja->num_cartas; i++){
		Carta *carta = &baraja->cartas[i];
		printf("%s de %s\n", carta->valor, carta->color);
	}

}



int main(){
	SetConsoleOutputCP(CP_UTF8); //gracias compañero de telegram

	Baraja baraja;
	leerCartas(&baraja);
	

	char opcion;
	do{
		puts("========================================");
		puts("     		Poker");
		puts("========================================");

		puts("1) cargar Cartas");
		puts("2) Buscar en Anchura");
		puts("3) Buscar Mejor Primero");
		puts("4) Salir");

		printf("Ingrese su opción: ");
		scanf(" %c", &opcion);

		switch (opcion){
		case '1':
			mostrarCartas(&baraja);
			barajarCartas(&baraja);
			break;
		case '2':
			// bfs(estado_inicial);
			break;
		case '3':
			// best_first(estado_inicial);
			break;
		}
		presioneTeclaParaContinuar();
		limpiarPantalla();

	} while (opcion != '4');

	return 0;
}
