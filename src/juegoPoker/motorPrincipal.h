#ifndef MOTORPRINCIPAL_H
#define MOTORPRINCIPAL_H

#include "estructuras.h"

//Funciones Auxiliares
void combinarCartasJugador(List *manoJugador, Mesa mesa, Carta cartasCombinadas[]);     //Combina las cartas del jugador con las de la mesa para formar una mano completa

int contarJugadoresPendientes(CList *jugadores, Jugador *actual);   //Cuenta los jugadores que están pendientes de actuar en la ronda actual, excluyendo al jugador actual

int obtenerApuestaMaxima(CList *jugadores);     //Obtiene la apuesta máxima entre todos los jugadores activos en la partida

void funcionTrampa(Partida *partida);   //Función de prueba para crear una situación específica en la partida (para pruebas y debugging)

//Funciones gestion mesa
void crearFlop(Partida *partida);   //Crea las 3 primeras cartas comunitarias (Flop) en la mesa de juego

void crearTurn(Partida *partida);   //Crea la cuarta carta comunitaria (Turn) en la mesa de juego

void crearRiver(Partida *partida);  //Crea la quinta y última carta comunitaria (River) en la mesa de juego

//Funciones Principales de la partida
void moverIzquierdaBoton(Partida *partida); //Mueve el botón del dealer a la izquierda, actualizando el jugador que será el siguiente en actuar

void definirGanador(Partida *partida);  //Determina el ganador de la partida al final de una ronda, evaluando las manos de los jugadores activos

static Accion tomarDecisiones(Partida *partida, Jugador *jugadorActual, int apuestaMax);    //Lógica para que un Bot tome decisiones basadas en su mano y la apuesta actual

void limpiarManos(Partida *partida);    //Limpia las manos de los jugadores al inicio de una nueva ronda, reiniciando sus estados y apuestas

void logicaJugador(Jugador *actual, int apuestaMax, Partida *partida, 
    int *jugadoresPendientes, int *salir, Jugador *inicio, int *cantidad, Jugador *jug);    //Lógica para que un jugador humano tome decisiones en su turno, mostrando opciones y procesando la entrada del usuario

void logicaBot(Jugador *actual, int *apuestaMax, Partida *partida, 
    int *jugadoresPendientes, int *salir, Jugador *inicio, int *cantidad, Jugador *jug);    //Lógica para que un Bot tome decisiones en su turno, simulando un comportamiento aleatorio y procesando la lógica de apuestas

void rondaDeApuestas(Partida *partida); //Gestiona una ronda de apuestas, permitiendo a los jugadores actuar en función de sus manos y las apuestas actuales

void iniciarRonda(Partida *partida);    //Inicia una ronda de juego, gestionando las apuestas iniciales, repartiendo cartas y llevando a cabo las rondas de apuestas

void iniciarPartida();  //Inicia una partida de poker, configurando los jugadores, la baraja y el estado inicial del juego

#endif