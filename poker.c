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
	Jugador *jugadorParte;
	Jugador *jugadorCiegaMayor;
	Jugador *jugadorCiegaMenor;
	Jugador *boton;
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

typedef struct{
	TipoMano tipo;
	int valores[5];
	int puntuacion;
} ManoEvaluada;

typedef struct{
	Carta cartas[7];
	int numCartas;
	ManoEvaluada mejorMano;
} GrafoMano;

int obtenerValorCarta(char *valor){
	if(strcmp(valor, "A") == 0) return 14;
	if(strcmp(valor, "K") == 0) return 13;
	if(strcmp(valor, "Q") == 0) return 12;
	if(strcmp(valor, "J") == 0) return 11;
	return atoi(valor);
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
}

ManoEvaluada evaluarMano(Carta cartas[], int numCartas) {
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

    return mano;
}


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
	//printf("\n");
}

int contarJugadoresYGandor(List *jugadores){
	int activos = 0;
	Jugador *ganador = NULL;
	Jugador * jug = list_first(jugadores);
	while (jug){
		if (strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0){
			activos++;
			ganador = jug;
		}
		jug = list_next(jugadores);
	}
	if (activos == 1 && ganador){
		strcpy(ganador->estado, "Ganador");
		printf("\n%s gana la ronda por retiro de los demas");
		return 1;
	}
	return 0;

}

void mostrarMesa(Mesa mesa)
{	
	printf("MESA ACTUAL:\n");
	switch (mesa.total)
	{
	case 3:
		printf("FLOP\n\n");
		break;
	case 4:
		printf("TURN\n\n");
		break;
	case 5:
		printf("RIVER\n\n");
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

int contarJugadoresActivos(CList *jugadores) {
    int n = 0;
    Jugador *jug = clist_first(jugadores);
    if (!jug) return 0;
    Jugador *inicio = jug;
    do {
        if (strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0) n++;
        jug = clist_next(jugadores);
    } while (jug != inicio);
    return n;
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

int contarJugadoresPendientes(CList *jugadores) {
    int n = 0;
    Jugador *jug = clist_first(jugadores);
    if (!jug) return 0;
    Jugador *inicio = jug;
    do {
        if (strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0 && !jug->yaActuo) n++;
        jug = clist_next(jugadores);
    } while (jug != inicio);
    return n;
}

void rondaDeApuestas(Partida *partida){ //reconocer si es humano o no
    int apuestaMax = obtenerApuestaMaxima(partida->jugadores);
    int jugadoresActivos = contarJugadoresActivos(partida->jugadores);
    int jugadoresPendientes = jugadoresActivos;

    Jugador *jug = clist_first(partida->jugadores);
    Jugador *inicio = jug;
    do {
        jug->yaActuo = 0;
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);

    Jugador *actual = partida->siguienteApuesta;
    inicio = actual;

    while (jugadoresPendientes > 0) {
        if (strcmp(actual->estado, "Jugando") == 0 && actual->fichas > 0) {
			mostrarMesa(partida->mesa);
            printf("\nTurno de %s\n\n", actual->nombre);
            mostrarMano(actual->mano);
            printf("\nFichas: %d | Apuesta actual: %d | Apuesta máxima: %d\n", actual->fichas, actual->apuesta, apuestaMax);

            int opcion, cantidad = 0;
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
                    if (actual->apuesta == apuestaMax) {
                        printf("%s pasa.\n", actual->nombre);
                    } else {
                        int diferencia = apuestaMax - actual->apuesta;
                        if (actual->fichas >= diferencia) {
                            actual->fichas -= diferencia;
                            actual->apuesta += diferencia;
                            partida->mesa.bote += diferencia;
                            printf("%s iguala la apuesta.\n", actual->nombre);
                        } else {
                            partida->mesa.bote += actual->fichas;
                            actual->apuesta += actual->fichas;
                            actual->fichas = 0;
                            printf("%s va all-in.\n", actual->nombre);
                        }
                    }
                    actual->yaActuo = 1;
                    jugadoresPendientes--;
                    break;

                case 2: // Raise
                    printf("¿Cuánto quieres subir? (mínimo %d): ", apuestaMax - actual->apuesta + 1);
                    scanf("%d", &cantidad);
                    if (cantidad > actual->fichas) cantidad = actual->fichas;
                    actual->fichas -= cantidad;
                    actual->apuesta += cantidad;
                    partida->mesa.bote += cantidad;
                    apuestaMax = actual->apuesta;
                    printf("%s sube la apuesta a %d.\n", actual->nombre, actual->apuesta);

                    // Reabrir ronda: todos deben responder al nuevo raise excepto quien lo hizo y los retirados
                    jug = clist_first(partida->jugadores);
                    inicio = jug;
                    do {
                        if (jug != actual && strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0)
                            jug->yaActuo = 0;
                        jug = clist_next(partida->jugadores);
                    } while (jug != inicio);
                    actual->yaActuo = 1;
                    jugadoresPendientes = contarJugadoresPendientes(partida->jugadores);
                    break;

                case 3: // Fold
                    strcpy(actual->estado, "Retirado");
                    printf("%s se retira.\n", actual->nombre);
                    actual->yaActuo = 1;
                    jugadoresPendientes--;
                    // Si solo queda uno, termina la ronda
                    if (contarJugadoresActivos(partida->jugadores) == 1) return;
                    break;
            }
        }
        // Avanza al siguiente jugador circularmente
        do {
            actual = clist_next(partida->jugadores);
        } while ((strcmp(actual->estado, "Retirado") == 0 || actual->fichas == 0 || actual->yaActuo) && actual != inicio);
		limpiarPantalla();
	}

    // Reinicia apuestas para la siguiente ronda
    jug = clist_first(partida->jugadores);
    inicio = jug;
    do {
        jug->apuesta = 0;
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
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
	partida.mesa.total = 0;
	

	barajarCartas(&partida.baraja); // Barajar cartas
	repartirCartas(&partida);
	rondaDeApuestas(&partida);
	//ver si todos foldearon
	crearFlop(&partida); //(3 cartas)
	rondaDeApuestas(&partida);
	//ver si todos foldearon
	crearTurn(&partida); //(1 carta)
	rondaDeApuestas(&partida);
	//ver si todos foldearon
	crearRiver(&partida); //(1 carta)
	
	rondaDeApuestas(&partida); //final
	printf("TERMINO\n\n");
	//ver si todos foldearon
	//definirGanador();
	//eliminarJugadores()-
	
}

void iniciarPartida(){
	char nombre[50];
	int jugadores;
	Partida partida;

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

    partida.boton = boton;
    partida.jugadorCiegaMenor = ciegaMenor;
    partida.jugadorCiegaMayor = ciegaMayor;
    partida.siguienteApuesta = siguienteApuesta;
    partida.jugadorParte = siguienteApuesta; // Si quieres que el que parte sea el primero en hablar

	partida.baraja = baraja;
	partida.ronda = 0;
	
	iniciarRonda(partida);

}

void eliminarJugadores(List *jugadores) // hacer posible mapa hash
{
	Jugador *jug = list_first(jugadores);
	while (jug)
	{
		if ( jug->fichas == 0 )
		{
			strcpy(jug->estado, "Eliminado");
		}
		jug = list_next(jugadores);
	}
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
