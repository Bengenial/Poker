/*#include <stdlib.h>
#include "estructuras.c"
#include "tdas/list.h"
#include "tdas/clist.h"
#include "tdas/heap.h"
#include "tdas/extra.h"


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

void combinarCartasJugador(List *manoJugador, Mesa mesa, Carta cartasCombinadas[]){
	int index = 0;
	Carta *carta = list_first(manoJugador);
	while(carta != NULL){
		cartasCombinadas[index] = *carta;
		index++;
		carta = list_next(manoJugador);
	}
	for (int i = 0; i < mesa.total; i++) {
		cartasCombinadas[index] = mesa.cartas[i];
		index++;
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

void mostrarCarta(Carta carta)
{
	if(strcmp(carta.color, "corazones") == 0) // son iguales
	{ 
		printf("\033[1;31m%s ♥\033[0m", carta.valor);
		return;
	}

	if(strcmp(carta.color, "diamantes") == 0)
	{ 
		printf("\033[1;34m%s ♦\033[0m", carta.valor);
		return;
	}
	
	if(strcmp(carta.color, "picas") == 0)
	{ 
		printf("\033[5;90m%s ♠\033[0m", carta.valor);
		return;
	}
	
	printf("\033[1;32m%s ♣\033[0m", carta.valor);
	
}

void mostrarCartasJugador(Jugador *jugador) {
    printf("%s: ", jugador->nombre);
    Carta *carta = list_first(jugador->mano);
    while (carta != NULL) {
        //printf("%s", carta->valor);
		mostrarCarta(*carta);
		printf("   ");
        carta = list_next(jugador->mano);
    }
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
		printf("Carta [%d]: ", k+1);//, mesa.cartas[k].valor);
		mostrarCarta(mesa.cartas[k]);
		printf("\n");
	}
	
}
*/