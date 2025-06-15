#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/clist.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>
#include <windows.h>
// id

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
	int esBot;
	char estado[10]; //jugando, retirado, ganador
	int apuesta;
	int yaActuo;
	int hizoRiseCall;
}Jugador;

typedef struct{
	Carta cartas[5]; //agregar de la baraja
	int total;
	int bote;
}Mesa;

typedef struct{
	CList *jugadores;
	int numJugadores;
	Jugador *siguienteApuesta;
	Jugador *jugadorCiegaMayor;
	Jugador *jugadorCiegaMenor;
	Jugador *jugadorBoton;
	Jugador *ganador;
	Baraja baraja;
	Mesa mesa;
	int ronda;
}Partida;

typedef enum {
    CARTA_ALTA = 0,
    PAR = 1,
    DOS_PARES = 2,
    TRIO = 3,
    ESCALERA = 4,
    COLOR = 5,
    FULL_HOUSE = 6,
    POKER_MANO = 7,
    ESCALERA_COLOR = 8,
    ESCALERA_REAL = 9
} TipoMano;

typedef enum{
	ACCION_FOLD,
	ACCION_CHECK,
	ACCION_CALL,
	ACCION_RAISE
}Accion;

typedef struct{
	TipoMano tipo;
	int valores[5];
	int puntuacion;
} ManoEvaluada;

typedef struct{
	Carta cartas[7]; //2 propias + 5 en mesa
	int numCartas; //numero total de cartas?
	ManoEvaluada mejorMano; //Resultado esperado de la mejor mano encontrada?
} GrafoMano;

int obtenerValorCarta(char *valor){
	if(strcmp(valor, "A") == 0) return 14;
	if(strcmp(valor, "K") == 0) return 13;
	if(strcmp(valor, "Q") == 0) return 12;
	if(strcmp(valor, "J") == 0) return 11;
	return atoi(valor); //otro valores (2-10)
}

int obtenerIndicePalo(char *color){
	if (strcmp(color, "corazones") == 0) return 0;
	if (strcmp(color, "diamantes") == 0) return 1;
	if (strcmp(color, "tréboles") == 0) return 2;
	if (strcmp(color, "picas") == 0) return 3;
	return -1;
}

int compararCartas(const void *a, const void *b)
{
	Carta *cartaA = (Carta*)a;
    Carta *cartaB = (Carta*)b;
    return obtenerValorCarta(cartaB->valor) - obtenerValorCarta(cartaA->valor);
}

int verificarColor(Carta cartas [], int numCartas, int *valoresColor)
{
	int palos[4] = {0};
	Carta cartasPorPalo[4][7];
	int contadorPorPalo[4] = {0};

	for (int i = 0; i < numCartas; i++){
		int palo = obtenerIndicePalo(cartas[i].color);
		if (palo >= 0 ){
			cartasPorPalo[palo][contadorPorPalo[palo]] = cartas[i];
			contadorPorPalo[palo]++;
			palos[palo]++; 
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if(palos[i] >= 5)
		{
			qsort(cartasPorPalo[i], contadorPorPalo[i], sizeof(Carta), compararCartas);
			for (int j = 0; j < 5; j++){
				valoresColor[j] = obtenerValorCarta(cartasPorPalo[i][j].valor);
			}
			return 1;
		}
	}
	return 0;
}

int verificarEscalera(Carta cartas[], int numCartas, int *valorEscalera) {
	int valores[15] = {0};
	for (int i = 0; i < numCartas; i++) {
        int valor = obtenerValorCarta(cartas[i].valor);
        valores[valor] = 1;
    }
	if (valores[14] && valores[2] && valores[3] && valores[4] && valores[5]){
		*valorEscalera = 5;
		return 1;
	}	
	for (int i = 14; i>=6; i--){
		if (valores[i] && valores[i-1] && valores[i-2] && valores[i-3] && valores[i-4]){
			*valorEscalera = i;
			return 1;
		}
	}
	return 0;;
}

void contarRepeticiones(Carta cartas[], int numCartas, int repeticiones[], int valores[]) {
    for (int i = 0; i < 15; i++) {
        repeticiones[i] = 0;
    }
    
    for (int i = 0; i < numCartas; i++) {
        int valor = obtenerValorCarta(cartas[i].valor);
        repeticiones[valor]++;
        valores[i] = valor;
    }
} //sirve para ver los pares y/o trios

ManoEvaluada evaluarMano(Carta cartas[], int numCartas) { //puede ser cartas[7]
    ManoEvaluada mano = {CARTA_ALTA, {0}, 0};

    qsort(cartas, numCartas, sizeof(Carta), compararCartas);

    int repeticiones[15] = {0};
    int valores[7];
    contarRepeticiones(cartas, numCartas, repeticiones, valores);

    int valoresColor[5];
    int esColor = verificarColor(cartas, numCartas, valoresColor);
    int valorEscalera;
    int esEscalera = verificarEscalera(cartas, numCartas, &valorEscalera);

    int pares = 0, trios = 0, pokers = 0;
    int valorPar[2] = {0}, valorTrio = 0, valorPoker = 0;
    int kickers[5] = {0};
    int numKickers = 0;

    for (int i = 14; i >= 2; i--) {
        if (repeticiones[i] == 4) {
            pokers++;
            valorPoker = i;
        } else if (repeticiones[i] == 3) {
            trios++;
            valorTrio = i;
        } else if (repeticiones[i] == 2) {
            if (pares < 2) {
                valorPar[pares] = i;
            }
            pares++;
        } else if (repeticiones[i] == 1 && numKickers < 5) {
            kickers[numKickers++] = i;
        }
    }

    if (esColor && esEscalera && valorEscalera == 14) {
        mano.tipo = ESCALERA_REAL;
        mano.valores[0] = 14;
        mano.puntuacion = 9000000;
    } else if (esColor && esEscalera) {
        mano.tipo = ESCALERA_COLOR;
        mano.valores[0] = valorEscalera;
        mano.puntuacion = 8000000 + valorEscalera * 10000;
    } else if (pokers > 0) {
        mano.tipo = POKER_MANO;
        mano.valores[0] = valorPoker;
        mano.valores[1] = kickers[0];
        mano.puntuacion = 7000000 + valorPoker * 10000 + kickers[0];
    } else if (trios > 0 && pares > 0) {
        mano.tipo = FULL_HOUSE;
        mano.valores[0] = valorTrio;
        mano.valores[1] = valorPar[0];
        mano.puntuacion = 6000000 + valorTrio * 10000 + valorPar[0] * 100;
    } else if (esColor) {
        mano.tipo = COLOR;
        for (int i = 0; i < 5; i++) {
            mano.valores[i] = valoresColor[i];
        }
        mano.puntuacion = 5000000 + valoresColor[0] * 10000 + valoresColor[1] * 1000 + 
                         valoresColor[2] * 100 + valoresColor[3] * 10 + valoresColor[4];
    } else if (esEscalera) {
        mano.tipo = ESCALERA;
        mano.valores[0] = valorEscalera;
        mano.puntuacion = 4000000 + valorEscalera * 10000;
    } else if (trios > 0) {
        mano.tipo = TRIO;
        mano.valores[0] = valorTrio;
        mano.valores[1] = kickers[0];
        mano.valores[2] = kickers[1];
        mano.puntuacion = 3000000 + valorTrio * 10000 + kickers[0] * 100 + kickers[1];
    } else if (pares >= 2) {
        mano.tipo = DOS_PARES;
        mano.valores[0] = valorPar[0] > valorPar[1] ? valorPar[0] : valorPar[1];
        mano.valores[1] = valorPar[0] < valorPar[1] ? valorPar[0] : valorPar[1];
        mano.valores[2] = kickers[0];
        mano.puntuacion = 2000000 + mano.valores[0] * 10000 + mano.valores[1] * 1000 + kickers[0];
    } else if (pares == 1) {
        mano.tipo = PAR;
        mano.valores[0] = valorPar[0];
        for (int i = 0; i < 3; i++) {
            mano.valores[i + 1] = kickers[i];
        }
        mano.puntuacion = 1000000 + valorPar[0] * 10000 + kickers[0] * 1000 + 
                         kickers[1] * 100 + kickers[2] * 10;
    } else {
        mano.tipo = CARTA_ALTA;
        for (int i = 0; i < 5; i++) {
            mano.valores[i] = kickers[i];
        }
        mano.puntuacion = kickers[0] * 10000 + kickers[1] * 1000 + kickers[2] * 100 + 
                         kickers[3] * 10 + kickers[4];
    }

    return mano; //returna 
}

/*Accion tomarDecisiones(ManoEvaluada manoActual, int apuestaActual)
{	
	//Tener en cuenta que la IA es lo más basico, osea va a ser predesible (por el momento...)
	
	if(mano.puntuacion < 1500000) //numero por definirse, pero es un ejemplo
	{ //pesima mano
	 	if(apuestaActual > 0) return ACCION_FOLD; //en este caso si aumentan la apuesta, me retiro (fold)
		else return ACCION_CHECK; si no aumentan la apuestaa, chekeamos (seguimos)
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

void repartirCartas(Partida *partida){
    Jugador *jug = clist_first(partida->jugadores);
    Jugador *inicio = jug;
    int i = 0;
    if (!jug) return;

    do {
        list_pushBack(jug->mano, &partida->baraja.cartas[i]);
        list_pushBack(jug->mano, &partida->baraja.cartas[i+1]);
        i += 2;
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
}

void mostrarPinta(Carta carta)
{
	if(strcmp(carta.color, "corazones") == 0) // son iguales
	{ 
		printf(" ♥️\n");
		return;
	}

	if(strcmp(carta.color, "diamantes") == 0)
	{ 
		printf(" ♦️\n");
		return;
	}
	
	if(strcmp(carta.color, "picas") == 0)
	{ 
		printf(" ♠️\n");
		return;
	}
	
	printf(" ♣️\n");
	
}

void mostrarMano(List *mano){
	Carta* carta = list_first(mano);

	printf("Tus cartas:\n");
	while(carta){
		printf("%s de %s", carta->valor, carta->color);
		mostrarPinta(*carta);
		carta = list_next(mano);
	}
}

void mostrarGandorFold(Partida *partida){
	
	printf("EL JUGADOR %s ha ganado %d fichas\n", partida->ganador->nombre, partida->mesa.bote);
	//repartir bote
	partida->ganador->fichas += partida->mesa.bote;
	partida->mesa.bote = 0;

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
		printf("Carta [%d]: %s %s", k+1, mesa.cartas[k].valor, mesa.cartas[k].color);
		mostrarPinta(mesa.cartas[k]);
	}
	
}

void crearFlop(Partida *partida){
	int i = partida->numJugadores * 2;

	for(int k = 0 ; k < 3 ; k++){	
		partida->mesa.cartas[k] = partida->baraja.cartas[i];
		i++;
	}
	partida->mesa.total = 3;
}

void crearTurn(Partida *partida){
	int i = partida->numJugadores * 2 + 3;
	
	partida->mesa.cartas[3] = partida->baraja.cartas[i];
	partida->mesa.total = 4;
}

void crearRiver(Partida *partida){
	int i = partida->numJugadores * 2 + 4;
	
	partida->mesa.cartas[4] = partida->baraja.cartas[i];
	partida->mesa.total = 5;
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


	//devolver actual (current?)
	do{
		if(jug == actual) break;
		jug = clist_next(jugadores);
	}while (jug != actual);
	
    return n;
}

void checkOrCall(Jugador *jugadorActual, int apuestaActual, Partida *partida, int *jugadoresPendientes){
	if (jugadorActual->apuesta == apuestaActual) {
		printf("%s pasa.\n", jugadorActual->nombre);
	} else {
		int diferencia = apuestaActual - jugadorActual->apuesta;
		if (jugadorActual->fichas >= diferencia) {
			jugadorActual->fichas -= diferencia;
			jugadorActual->apuesta += diferencia;
			partida->mesa.bote += diferencia;
			printf("%s iguala la apuesta.\n", jugadorActual->nombre);
		} else {
			partida->mesa.bote += jugadorActual->fichas;
			jugadorActual->apuesta += jugadorActual->fichas;
			jugadorActual->fichas = 0;
			printf("%s va all-in.\n", jugadorActual->nombre);
		}
	}
	jugadorActual->yaActuo = 1;
	jugadorActual->hizoRiseCall = 1;
	(*jugadoresPendientes)--;
}


void raise(Jugador *actual, int *apuestaMax, Partida *partida, int *jugadoresPendientes, Jugador *inicio, int  *cantidad, Jugador *jug){
	printf("¿Cuánto quieres subir? (mínimo %d): ", (*apuestaMax) - actual->apuesta + 1);
	scanf("%d", &(*cantidad));
	if ((*cantidad) > actual->fichas) (*cantidad) = actual->fichas;
	actual->fichas -= (*cantidad);
	actual->apuesta += (*cantidad);
	partida->mesa.bote += (*cantidad);
	(*apuestaMax) = actual->apuesta;
	actual->hizoRiseCall = 1;
	printf("%s sube la apuesta a %d.\n", actual->nombre, actual->apuesta);

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
}

void fold(Jugador *actual, int *jugadoresPendientes, Partida *partida, int *salir, Jugador *inicio){
	//nuevo
	strcpy(actual->estado, "Retirado");
	printf("%s se retira.\n", actual->nombre);
	actual->yaActuo = 1;

	//solo restar si hay más de un jugador
	(*jugadoresPendientes)--;
	// Si solo queda uno, termina la ronda
	if (contarJugadoresActivos(partida->jugadores, actual) == 1){
		do {
			actual = clist_next(partida->jugadores);
		} while ((strcmp(actual->estado, "Retirado") == 0 ||actual->fichas == 0 || actual->yaActuo) && actual != inicio);
		partida->ganador = actual;
		(*salir) = 0;
	}
}

int obtenerApuestaMaxima(CList *jugadores) {
    int max = 0;
    Jugador *jug = clist_first(jugadores);
    if (!jug) return 0;
    Jugador *inicio = jug;
    do {
        if (jug->apuesta > max) max = jug->apuesta;
        jug = clist_next(jugadores);
    } while (jug != inicio);
    return max;
}

void rondaDeApuestas(Partida *partida){ //reconocer si es humano o no
    int apuestaMax = obtenerApuestaMaxima(partida->jugadores);
    int jugadoresActivos = contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores));
    int jugadoresPendientes = jugadoresActivos;

    Jugador *jug = clist_first(partida->jugadores);
    Jugador *inicio = jug;
	
    do {
		//printf("JUGADOR = %s\n", jug->nombre);
        jug->yaActuo = 0;
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
	

    Jugador *actual = clist_first(partida->jugadores);
    inicio = actual;
	while (actual != partida->siguienteApuesta){
		actual = clist_next(partida->jugadores);
		if (actual == inicio) break;
	}

	inicio = actual;
	int salir = 1;

	//benjoid
    while(jugadoresPendientes > 0 && salir){
		printf("JUGADORES PENDIENTES = %d\n", jugadoresPendientes);

		if (strcmp(actual->estado, "Jugando") == 0 && actual->fichas > 0) {
			int opcion, cantidad = 0;
			mostrarMesa(partida->mesa);
			printf("\nTurno de %s\n", actual->nombre);
			if (actual == partida->jugadorBoton) printf("BOTÓN\n\n");
			if (actual == partida->jugadorCiegaMenor) printf("CIEGA MENOR\n\n");
			if (actual == partida->jugadorCiegaMayor) printf("CIEGA MAYOR\n\n");
			else printf("\n\n");
			
			mostrarMano(actual->mano);
			printf("\nFichas: %d | Apuesta actual: %d | Apuesta máxima: %d\n", actual->fichas, actual->apuesta, apuestaMax);

			if (actual->hizoRiseCall){
				if (actual->apuesta == apuestaMax){
					printf("Opciones: [1] Pasar (check) [2] Retirarse (fold)\n");
				} else {
					printf("Opciones: [1] Igualar (call) [2] Retirarse (fold)\n");
				}

				do {
					printf("Elige una opción: ");
					if (scanf("%d", &opcion) != 1) {
						while (getchar() != '\n');
						opcion = 0;
					}
				} while (opcion < 1 || opcion > 2);

				switch (opcion) {
					case 1: // check o call
						checkOrCall(actual, apuestaMax, partida, &jugadoresPendientes);
						break;

					case 2: // Fold
						fold(actual, &jugadoresPendientes, partida, &salir, inicio);
						break;
				}
			}
			else{
				if (actual->apuesta == apuestaMax) {
					printf("Opciones: [1] Pasar (check) [2] Subir (raise) [3] Retirarse (fold)\n");
				} else {
					printf("Opciones: [1] Igualar (call) [2] Subir (raise) [3] Retirarse (fold)\n");
				}

				do {
					printf("Elige una opción: ");
					if (scanf("%d", &opcion) != 1) {
						while (getchar() != '\n');
						opcion = 0;
					}
				} while (opcion < 1 || opcion > 3);

				switch (opcion) {
					case 1: // check o call
						checkOrCall(actual, apuestaMax, partida, &jugadoresPendientes);
						break;

					case 2: // Raise
						raise(actual, &apuestaMax, partida, &jugadoresPendientes, inicio, &cantidad, jug);
						
						/*printf("¿Cuánto quieres subir? (mínimo %d): ", apuestaMax - actual->apuesta + 1);
						scanf("%d", &cantidad);
						if (cantidad > actual->fichas) cantidad = actual->fichas;
						actual->fichas -= cantidad;
						actual->apuesta += cantidad;
						partida->mesa.bote += cantidad;
						apuestaMax = actual->apuesta;
						printf("%s sube la apuesta a %d.\n", actual->nombre, actual->apuesta);

						// Reabrir ronda: todos deben responder al nuevo raise excepto quien lo hizo y los retirados
						jug = clist_first(partida->jugadores);
						Jugador *inicio2 = jug;
						do {
							if (jug != actual && strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0)
								jug->yaActuo = 0;
							jug = clist_next(partida->jugadores);
						} while (jug != inicio2);
						actual->yaActuo = 1;
						jugadoresPendientes = contarJugadoresPendientes(partida->jugadores, actual);
						inicio = actual;*/
						break;
						

					case 3: // Fold
						fold(actual, &jugadoresPendientes, partida, &salir, inicio);
						break;
				}
			}
			
		}
		// Avanza al siguiente jugador circularmente
		//actual = clist_next(partida->jugadores);
		//if (actual != inicio) break;
		
		do {
			actual = clist_next(partida->jugadores);
		} while ((strcmp(actual->estado, "Retirado") == 0 ||actual->fichas == 0 || actual->yaActuo) && actual != inicio);
		
		limpiarPantalla();
	}
	

    // Reinicia apuestas para la siguiente ronda
    
	jug = clist_first(partida->jugadores);
    inicio = jug;
    do {
        jug->apuesta = 0;
		jug->hizoRiseCall = 0;
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
	
}

void iniciarRonda(Partida *partida, int IArand){
	int ciegaMayor = 10;
	int ciegaMenor = 5;
	partida->mesa.bote = 0;
	
	//Ciega Mayor
	if (partida->jugadorCiegaMayor && partida->jugadorCiegaMayor->fichas >= ciegaMayor){
        partida->jugadorCiegaMayor->fichas -= ciegaMayor;
		partida->jugadorCiegaMayor->apuesta = ciegaMayor;
	}
	else if (partida->jugadorCiegaMayor){
		ciegaMayor = partida->jugadorCiegaMayor->fichas;
		partida->jugadorCiegaMayor->apuesta = ciegaMayor;
		partida->jugadorCiegaMayor = 0;
	}

	//CIega Menor
    if (partida->jugadorCiegaMenor && partida->jugadorCiegaMenor->fichas >= ciegaMenor){
        partida->jugadorCiegaMenor->fichas -= ciegaMenor;
		partida->jugadorCiegaMenor->apuesta = ciegaMenor;
	}
	else if (partida->jugadorCiegaMenor){
		ciegaMenor = partida->jugadorCiegaMenor->fichas;
		partida->jugadorCiegaMenor->fichas = 0;
		partida->jugadorCiegaMenor->apuesta = ciegaMenor;
	}

	partida->mesa.bote += ciegaMayor + ciegaMenor;
	partida->mesa.total = 0;
	
	
	//ahora (culpa anselmo)
	barajarCartas(&partida->baraja); // Barajar cartas
	repartirCartas(partida);
	rondaDeApuestas(partida);//agregar IArand dsp
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		return;
	}	//foldearon
	//mover a laizquierda del boton

	crearFlop(partida); //(3 cartas)
	rondaDeApuestas(partida);
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		return;
	}	//foldearon
	//mover a laizquierda del boton

	crearTurn(partida); //(1 carta)
	rondaDeApuestas(partida);
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		return;
	}	//foldearon
	//mover a laizquierda del boton

	crearRiver(partida); //(1 carta)
	rondaDeApuestas(partida); //final
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		return;
	}	//foldearon
	printf("TERMINO\n\n");

	//definirGanador(&partida);
	//eliminarJugadores()
	
}

void iniciarPartida(int IArand){
	char nombre[50];
	int jugadores;
	Partida partida;
	//testeo de elecciones
	IArand = 0;

	partida.jugadores = clist_create();

	puts("Tu nombre:");
	fgets(nombre, sizeof(nombre), stdin);
	nombre[strcspn(nombre, "\n")] = 0;
	
	
	do{
		puts("Cuántos jugadores bots van a jugar? (1-9)"); //bots
		scanf("%i",&jugadores);
		getchar();

	}while(jugadores < 1 || jugadores > 9);

	limpiarPantalla();

	for (int i = 1; i <= jugadores; i++){
		char nomBot[10];
		sprintf(nomBot, "Bot %d", i);
		clist_pushBack(partida.jugadores, crearJugador(nomBot, 1));
	}

	clist_pushBack(partida.jugadores, crearJugador(nombre, 0));
	

	Baraja baraja;
	leerCartas(&baraja);

	partida.numJugadores = jugadores + 1;

	Jugador *boton = clist_first(partida.jugadores); // Botón
    Jugador *ciegaMenor = clist_next(partida.jugadores); // Ciega menor
    Jugador *ciegaMayor = clist_next(partida.jugadores); // Ciega mayor
    Jugador *siguienteApuesta = clist_next(partida.jugadores); // Primer jugador en hablar preflop

    partida.jugadorBoton = boton;
    partida.jugadorCiegaMenor = ciegaMenor;
    partida.jugadorCiegaMayor = ciegaMayor;
    partida.siguienteApuesta = siguienteApuesta; //parte

	partida.baraja = baraja;
	partida.ronda = 0;
	
	
	Jugador *jug = siguienteApuesta;
    Jugador *inicio = jug;

    do {
        printf("JUGADOR = %s es ", jug->nombre);
		if(jug == partida.jugadorBoton) printf("BOTON\n");
		else if (jug == partida.jugadorCiegaMayor) printf("CIEGA MAYOR\n");
		else if (jug == partida.jugadorCiegaMenor) printf("CIEGA MENOR\n");
		else if (jug == partida.siguienteApuesta) printf("EMPIEZA\n");
		else printf("NORMAL\n");

		jug = clist_next(partida.jugadores);
    } while (jug != inicio);

	//antes iniciarRonda(partida,IArand);
	iniciarRonda(&partida, IArand);

}
/*
int todosFold(Partida partida)
{
	Jugador *jug = clist_first(partida.jugadores);
	Jugador *inicio = jug;
	int totaljug = partida.numJugadores - 1;
	int totalret = 0;
	do{
		
		if ( strcmp(jug->estado, "Retirado") )
		{
			totalret++;
		}
		if ( totalret == totaljug )
		{
			return 1;
		}
		jug = clist_next(jugadores);
	} while ( jug != inicio );
	return 0;
}*/

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
void eliminarJugadores(CList *jugadores) // hacer posible mapa hash
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

void intro(){
    printf("\n");

    printf("  ____   ___   _  __ _____ ____  \n");
    Sleep(300); // 300 ms, funcionde <windows.h>

    printf(" |  _ \\ / _ \\ | |/ /| ____|  _ \\ \n");
    Sleep(300);

    printf(" | |_) | | | || ' / |  _| | |_) |\n");
    Sleep(300);

    printf(" |  __/| |_| || . \\ | |___|  _ < \n");
    Sleep(300);

    printf(" |_|    \\___/ |_|\\_\\|_____|_| \\_\\\n");
    Sleep(3000); //300

    printf("\n\n");
}

int main(){
	SetConsoleOutputCP(CP_UTF8); //gracias compañero de telegram

	limpiarPantalla();
	//intro();
	limpiarPantalla();
	int IArand;

	char opcion;
	do{
		puts("========================================");
		puts("           ♠️  ♥️  Poker  ♦️  ♣️");
		puts("========================================");

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
