#include <stdlib.h>

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
	Jugador *siguienteApuesta; //siguiente en partir
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

typedef struct {
    Jugador *jugador;
    ManoEvaluada mano;
} JugadorEvaluado;