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
	Carta cartas[52]; // ver si es factible hacerlo con un array o una lista //o quizas una cola...
	int num_cartas;
}Baraja;

typedef struct{
	char nombre[50];
	int fichas;
	List *mano;
	char estado[10]; //jugando, retirado, ganador
	int apuesta;
}Jugador;

typedef struct{
	Carta cartas[5];
	int bote;
}Mesa;

typedef struct{
	List *jugadores;
	int numJugadores;
	Jugador *jugadorActual;
	Jugador *jugadorCiegaMayor;
	Jugador *jugadorCiegaMenor;
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

void mostrarCartas(Baraja *baraja){	//Para testeo
	for (int i = 0; i < baraja->num_cartas; i++){
		Carta *carta = &baraja->cartas[i];
		printf("%s de %s\n", carta->valor, carta->color);
	}

}

Jugador *crearJugador(char *nombre){
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
	return jugador;
}

void repartirCartas(Partida partida){
	Jugador *jug = list_first(partida.jugadores);
	int i = 0;
	while (jug){
		list_pushBack(jug->mano, &partida.baraja.cartas[i]);
		list_pushBack(jug->mano, &partida.baraja.cartas[i+1]);
		i += 2;
		jug = list_next(partida.jugadores);
	}
}

void mostrarMano(List *mano){
	Carta* carta = list_first(mano);

	printf("Tus cartas:\n");
	while(carta){
		printf("%s de %s\n", carta->valor, carta->color);
		carta = list_next(mano);
	}
	//printf("\n");
}

void rondaDeApuestas(Partida *partida, int apuesta){
	int apuestaMax = apuesta;
	int jugadoresActivos;
	int cambios;
	
	do{
		jugadoresActivos = 0;
		cambios = 0;
		Jugador *jug = list_first(partida->jugadores);
		while (jug){
			if (strcmp(jug->estado, "Retirado") != 0 && jug->fichas >0){
				printf("\nTurno de %s\n\n", jug->nombre);
				mostrarMano(jug->mano);
				
				printf("\nFichas: %d | Apuesta actual: %d | Apuesta máxima: %d\n", jug->fichas, jug->apuesta, apuestaMax);

				if (jug->apuesta == apuestaMax){
					printf("Opciones: [1] Check | [2] Raise | [3] Fold\n");
				} else{
					printf("Opciones: [1] Call | [2] Raise | [3] Fold\n");
				}

				int opcion, cantidad = 0;
				do {
					if (scanf("%d", &opcion) != 1) {
						while (getchar() != '\n'); // Limpia el buffer si hay entrada inválida
						opcion = 0; // Fuerza a repetir el ciclo
					}
					if (opcion < 1 || opcion > 3) {
						printf("Opción no válida. Intenta de nuevo.\n");
					}
				} while (opcion < 1 || opcion > 3);

				switch (opcion){
					case 1: //Check o call
						if (jug->apuesta == apuestaMax){
							printf("%s pasa", jug->nombre);
						} else{
							int diferencia = apuestaMax - jug->apuesta;
							if(jug->fichas >= diferencia){
								jug->fichas -= diferencia;
								jug->apuesta += diferencia;
								partida->mesa.bote += diferencia;
								printf("%s iguala la apuesta\n", jug->nombre);
							} else{
								partida->mesa.bote += jug->fichas;
								jug->apuesta += jug->fichas;
								jug->fichas = 0;
								printf("%s va all-in\n", jug->nombre);
							}
							cambios = 1;
						}
						break;
					case 2: //Raise
						printf("¿Cuánto quieres subir? (mínimo %d): ", apuestaMax -jug->apuesta + 1);
						scanf("%d", &cantidad);
						if (cantidad > jug->fichas){
							cantidad = jug->fichas;
							jug->fichas = 0;
							jug->apuesta += cantidad;
							printf("%s va all-in con %d fichas\n", jug->nombre, jug->apuesta);
						} else{
							jug->fichas -= cantidad;
							jug->apuesta += cantidad;
							printf("%s sube la apuesta a %d\n", jug->nombre, jug->apuesta);
						}
						partida->mesa.bote += cantidad;
						if (jug->apuesta > apuestaMax){
							apuestaMax = jug->apuesta;
							cambios = 1;
						}
						break;
					case 3: //Fold
						strcpy(jug->estado, "Retirado");
						printf("%s se retira\n", jug->nombre);
						break;
				}
				jugadoresActivos++;
			}
			jug = list_next(partida->jugadores);
		}
	}while (cambios);

	Jugador *jug = list_first(partida->jugadores);
	while (jug){
		jug->apuesta = 0;
		jug = list_next(partida->jugadores);
	}
}

void iniciarRonda(Partida partida){
	int ciegaMayor = 10;
	int ciegaMenor = 5;
	
	//Ciega Mayor
	if (partida.jugadorCiegaMayor && partida.jugadorCiegaMayor->fichas >= ciegaMayor){
        partida.jugadorCiegaMayor->fichas -= ciegaMayor;
		partida.jugadorCiegaMayor->apuesta = ciegaMayor;
	}
	else if (partida.jugadorCiegaMayor){
		ciegaMayor = partida.jugadorCiegaMayor->fichas;
		partida.jugadorCiegaMayor->apuesta = ciegaMayor;
		partida.jugadorCiegaMayor = 0;
	}

	//CIega Menor
    if (partida.jugadorCiegaMenor && partida.jugadorCiegaMenor->fichas >= ciegaMenor){
        partida.jugadorCiegaMenor->fichas -= ciegaMenor;
		partida.jugadorCiegaMenor->apuesta = ciegaMenor;
	}
	else if (partida.jugadorCiegaMenor){
		ciegaMenor = partida.jugadorCiegaMenor->fichas;
		partida.jugadorCiegaMenor->fichas = 0;
		partida.jugadorCiegaMenor->apuesta = ciegaMenor;
	}

	partida.mesa.bote += ciegaMayor + ciegaMenor;
	

	barajarCartas(&partida.baraja); // Barajar cartas
	repartirCartas(partida);
	rondaDeApuestas(&partida,ciegaMayor);
	//mostrarFlop
	//rondaDeApuestas(&partida);
	//mostrarTurn
	//rondaDeApuestas(&partida);
	//mostrarRiver
	//rondaDeApuestas(&partida); //final
	//definirGanador();
	//eliminarJugadores()
	
}

void iniciarPartida(){
	char nombre[50];
	int jugadores;
	Partida partida;

	partida.jugadores = list_create();

	puts("Tu nombre:");
	fgets(nombre, sizeof(nombre), stdin);
	nombre[strcspn(nombre, "\n")] = 0;
	
	list_pushBack(partida.jugadores, crearJugador(nombre));
	
	do{
		puts("Cuántos jugadores bots van a jugar? (2-9)"); //bots
		scanf("%i",&jugadores);
		getchar();

	}while(jugadores < 2 || jugadores > 9);

	

	for (int i = 1; i <= jugadores; i++){
		char nomBot[10];
		sprintf(nomBot, "Bot %d", i);
		list_pushBack(partida.jugadores, crearJugador(nomBot));
	}

	Baraja baraja;
	leerCartas(&baraja);

	Jugador *aux = list_first(partida.jugadores);

	partida.numJugadores = jugadores + 1;
	partida.jugadorActual = aux;

	aux = list_next(partida.jugadores);
	partida.jugadorCiegaMayor = aux;

	aux = list_next(partida.jugadores);
	partida.jugadorCiegaMenor = aux;

	partida.baraja = baraja;
	partida.ronda = 0;
	
	iniciarRonda(partida);

}

int main(){
	SetConsoleOutputCP(CP_UTF8); //gracias compañero de telegram

	char opcion;
	do{
		puts("========================================");
		puts("           ♠️  ♥️  Poker  ♦️  ♣️");
		puts("========================================");

		puts("1) Iniciar Partida");
		puts("2) Salir");

		printf("Ingrese su opción: ");
		scanf(" %c", &opcion);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);

		switch (opcion){
		case '1':
			iniciarPartida();

			//mostrarCartas(&baraja);
			//barajarCartas(&baraja);
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
