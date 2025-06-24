#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct {
    int id;
    char color[10];
    char valor[3];
} Carta;

typedef struct {
    Carta cartas[52];
    int num_cartas;
} Baraja;

typedef enum {
    CARTA_ALTA = 0, PAR = 1, DOS_PARES = 2, TRIO = 3, ESCALERA = 4,
    COLOR = 5, FULL_HOUSE = 6, POKER_MANO = 7, ESCALERA_COLOR = 8,
    ESCALERA_REAL = 9
} TipoMano;

typedef enum {
    ACCION_FOLD, ACCION_CHECK, ACCION_CALL, ACCION_RAISE
} Accion;

typedef struct {
    TipoMano tipo;
    int valores[5];
    int puntuacion;
} ManoEvaluada;

typedef struct {
    char nombre[50];
    int fichas;
    List *mano;
    int esBot;
    char estado[15];
    int apuesta;
    int yaActuo;
    int hizoRiseCall;
} Jugador;

typedef struct {
    Jugador *jugador;
    ManoEvaluada mano;
} JugadorEvaluado;

typedef struct {
    Carta cartas[5];
    int total;
    int bote;
} Mesa;

typedef struct {
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