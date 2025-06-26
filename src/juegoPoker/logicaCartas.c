#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//TDAS
#include "../tdas/extra.h"

//JUEGOPOKER
#include "estructuras.h"
#include "accionesJugador.h"


//Funciones de la baraja en sí
void intercambiarCartas(Carta *a, Carta *b){
	Carta aux = *a;
    *a = *b;
    *b = aux;
}

void leerCartas(Baraja *baraja){
	FILE *archivo = fopen("src/data/cartas_poker.csv", "r");
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


//Logica de evaluciones
int obtenerValorCarta(char *valor){
	if(strcmp(valor, "A") == 0) return 14;
	if(strcmp(valor, "K") == 0) return 13;
	if(strcmp(valor, "Q") == 0) return 12;
	if(strcmp(valor, "J") == 0) return 11;
	return atoi(valor); //otro valores (2-10)
}

int compararCartas(const void *a, const void *b)
{
	Carta *cartaA = (Carta*)a;
    Carta *cartaB = (Carta*)b;
    return obtenerValorCarta(cartaB->valor) - obtenerValorCarta(cartaA->valor);
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

int obtenerIndicePalo(char *color){
	if (strcmp(color, "corazones") == 0) return 0;
	if (strcmp(color, "diamantes") == 0) return 1;
	if (strcmp(color, "tréboles") == 0) return 2;
	if (strcmp(color, "picas") == 0) return 3;
	return -1;
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

    return mano; //retorna 
}

