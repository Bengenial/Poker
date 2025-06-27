#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include "../tdas/list.h"
#include "../tdas/clist.h"

typedef struct {        //representa una carta del juego
    int id;
    char color[10];
    char valor[3];
} Carta;

typedef struct {        //representa una baraja de cartas
    Carta cartas[52];
    int num_cartas;
} Baraja; //

typedef enum {          //representa los tipos de manos en el poker
    CARTA_ALTA = 0, PAR = 1, DOS_PARES = 2, TRIO = 3, ESCALERA = 4,
    COLOR = 5, FULL_HOUSE = 6, POKER_MANO = 7, ESCALERA_COLOR = 8,
    ESCALERA_REAL = 9
} TipoMano;

typedef enum {          //representa las acciones que un jugador puede realizar
    ACCION_FOLD, ACCION_CHECK, ACCION_CALL, ACCION_RAISE
} Accion;

typedef struct {        //representa una mano evaluada del jugador
    TipoMano tipo;
    int valores[5];
    int puntuacion;
} ManoEvaluada;

typedef struct {        //representa un jugador en el juego
    char nombre[50];
    int fichas;
    List *mano;
    int esBot;
    char estado[15];
    int apuesta;
    int yaActuo;
    int hizoRiseCall;
    int allIn[2]; //primera casilla, va a ser 0 o 1, dependiendo si hiso all in o no, 
                  //la segunda va a ser la cantidad del all in que hiso
} Jugador;

typedef struct {        //representa un jugador evaluado con su mano y puntuación
    Jugador *jugador;
    ManoEvaluada mano;
} JugadorEvaluado;

typedef struct {        //representa la mesa de juego
    Carta cartas[5];
    int total;
    int bote;
} Mesa;

typedef struct {        //representa una partida de poker
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
} Partida;


#endif