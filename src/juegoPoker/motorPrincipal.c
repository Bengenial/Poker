#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//TDAS
#include "../tdas/extra.h"

//JUEGOPOKER
#include "motorPrincipal.h"
#include "logicaCartas.h"
#include "accionesJugador.h"
#include "visualizacion.h"

//Funciones Auxiliares

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

void funcionTrampa(Partida *partida){ //funciona para 3 jugadores
	Carta carta;
	//1 JUG
	strcpy(carta.valor,"A");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[0] = carta;

	strcpy(carta.valor,"3");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[1] = carta;

	//2 JUG
	strcpy(carta.valor,"A");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[2] = carta;

	strcpy(carta.valor,"3");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[3] = carta;

	//3 JUG
	strcpy(carta.valor,"2");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[4] = carta;

	strcpy(carta.valor,"3");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[5] = carta;

	//MESA
	strcpy(carta.valor,"10");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[6] = carta;

	strcpy(carta.valor,"7");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[7] = carta;

	strcpy(carta.valor,"J");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[8] = carta;

	strcpy(carta.valor,"K");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[9] = carta;

	strcpy(carta.valor,"8");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[10] = carta;
}


//Funciones gestion mesa
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


//Funciones Principales de la partida
void moverIzquierdaBoton(Partida *partida){
	
	Jugador *boton = clist_first(partida->jugadores);
	Jugador *inicio = boton;

	while(boton != partida->jugadorBoton){
		boton = clist_next(partida->jugadores);
		if(boton == inicio) break;
	}
	
	do{
		boton = clist_next(partida->jugadores);
		if (strcmp(boton->estado, "Jugando") == 0 && boton->fichas > 0 )
		{	
			partida->siguienteApuesta = boton;
			break;
		}

	} while(boton != partida->jugadorBoton);

}


void definirGanador(Partida *partida){
	printf("\n=== SHOWDOWN ===\n");
	Sleep(1000);
	mostrarMesa(partida);
	Sleep(1000);
	printf("\n");
	
	JugadorEvaluado jugadoresEvaluados[10];
	int numJugadoresActivos = 0;
	Jugador *jug = clist_first(partida->jugadores);
	Jugador *inicio = jug;
	do {
        if (strcmp(jug->estado, "Jugando") == 0) {

			//parte donde evaluamos la mano 
            Carta cartasCombinadas[7]; 
            combinarCartasJugador(jug->mano, partida->mesa, cartasCombinadas);
            jugadoresEvaluados[numJugadoresActivos].jugador = jug;
            jugadoresEvaluados[numJugadoresActivos].mano = evaluarMano(cartasCombinadas, 7);
			jugadoresEvaluados[numJugadoresActivos].apuestaRestante = jug->apuesta;//nuevo para el bote
            mostrarCartasJugador(jug);
            printf("- ");
            mostrarTipoMano(jugadoresEvaluados[numJugadoresActivos].mano.tipo);
            printf(" (Puntuación: %d)\n", jugadoresEvaluados[numJugadoresActivos].mano.puntuacion);
            numJugadoresActivos++;
        }
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
	if (numJugadoresActivos == 0) {
        printf("Error: No hay jugadores activos\n");
        return;
    }
	

	//bruno
	int mejorPuntuacion = jugadoresEvaluados[0].mano.puntuacion;
	for (int i = 1; i < numJugadoresActivos; i++) {
        if (jugadoresEvaluados[i].mano.puntuacion > mejorPuntuacion) {
            mejorPuntuacion = jugadoresEvaluados[i].mano.puntuacion;
        }
    }
	int numGanadores = 0;
    Jugador *ganadores[10];
	for (int i = 0; i < numJugadoresActivos; i++) {
        if (jugadoresEvaluados[i].mano.puntuacion == mejorPuntuacion) {
            ganadores[numGanadores] = jugadoresEvaluados[i].jugador;
            numGanadores++;
        }
    }
	printf("\n===RESULTADO===\n");
	if(numGanadores == 1){
		printf("Ganador: %s\n", ganadores[0]->nombre);
		printf("Gana %d fichas\n", partida->mesa.bote);
		for (int i = 0; i < numJugadoresActivos; i++) {
            if (jugadoresEvaluados[i].jugador == ganadores[0]) {
                mostrarTipoMano(jugadoresEvaluados[i].mano.tipo);
                break;
            }
        }
        printf("\n");
		ganadores[0]->fichas += partida->mesa.bote;
		partida->ganador = ganadores[0];
	}
	else{
		printf("Empate entre %d jugadores:\n", numGanadores);
		int fichasPorJugador = partida->mesa.bote/numGanadores;
		int resto = partida->mesa.bote % numGanadores;
		for (int i = 0; i < numGanadores; i++){
			printf(" - %s", ganadores[i]->nombre);
			int fichasARecibir = fichasPorJugador;
			if (i == 0) fichasARecibir += resto;
			ganadores[i]->fichas += fichasARecibir;
			printf(" recibe %d fichas\n", fichasARecibir);

		}
		partida->ganador = ganadores[0];
	}
	partida->mesa.bote = 0; // Reiniciar el bote para la siguiente ronda
	printf("\nFichas después de la mano:\n");
    jug = clist_first(partida->jugadores);
    inicio = jug;
    do {
        printf("%s: %d fichas", jug->nombre, jug->fichas);
		if(jug->fichas == 0) printf("\033[5;91m BUSTED\033[0m");
		puts("");
        jug = clist_next(partida->jugadores);
    } while (jug != inicio); ///

}

void limpiarManos(Partida *partida){
	Jugador *jug = clist_first(partida->jugadores);
	Jugador *inicio = jug;
	do{
		while(list_first(jug->mano) != NULL){
			list_popFront(jug->mano); //limpiar mano
		}
		if(jug->fichas > 0){
			strcpy(jug->estado, "Jugando");
		}
		else{
			strcpy(jug->estado, "Eliminado");
		}
		jug->apuesta = 0;
		jug->yaActuo = 0;
		jug->hizoRiseCall = 0;
		jug = clist_next(partida->jugadores);
	} while(jug != inicio);
}

Accion tomarDecisiones(Partida *partida, Jugador *jugadorActual, int apuestaMax) // las decisiones de un bot
{
	int apuestaActual = jugadorActual->apuesta;
    ManoEvaluada manoEvaluada;
    Accion accionDeseada; // Aquí guardaremos la acción ideal del bot

	/* //? para debuguear, por si quieres que todos los bots te siguan el call
	if (apuestaActual < apuestaMax) 
        return ACCION_CALL;
    else 
        return ACCION_CHECK;
	*/

	/* //? para debuguear, por si quieres que todos los bots se retiren
	return ACCION_FOLD;
	*/

    // El bot decide qué quiere hacer basándose en sus cartas.

    if (partida->mesa.total == 0) { // --- LÓGICA PRE-FLOP ---
        Carta *c1 = list_first(jugadorActual->mano);
        Carta *c2 = list_next(jugadorActual->mano);
        int val1 = obtenerValorCarta(c1->valor);
        int val2 = obtenerValorCarta(c2->valor);

        // Nivel 1: Manos Premium -> Desea hacer RAISE
        if ((val1 == val2 && val1 >= 11) || (val1 >= 13 && val2 >= 12 && strcmp(c1->color, c2->color) == 0)) {
            accionDeseada = ACCION_RAISE;
        }
        // Nivel 2: Manos Buenas -> Desea hacer CALL o CHECK
        else if ((val1 == val2 && val1 >= 7) || (val1 >= 10 && val2 >= 9 && strcmp(c1->color, c2->color) == 0)) {
            accionDeseada = (apuestaActual < apuestaMax) ? ACCION_CALL : ACCION_CHECK;
        }
        // Nivel 3: Manos Especulativas -> Depende del costo
        else {
            if (apuestaActual < apuestaMax) { // Si hay apuesta
                // Esta es tu lógica corregida, ¡y es correcta!
                if (apuestaMax <= partida->mesa.ciegaMayor) {
                    accionDeseada = ACCION_CALL; // Paga si solo es la ciega
                } else {
                    accionDeseada = ACCION_FOLD; // Se retira si es una subida real
                }
            } else {
                accionDeseada = ACCION_CHECK; // Si no hay apuesta, pasa
            }
        }
    }
    else { // --- LÓGICA POST-FLOP ---
        Carta cartasCombinadas[7];
        combinarCartasJugador(jugadorActual->mano, partida->mesa, cartasCombinadas);
        manoEvaluada = evaluarMano(cartasCombinadas, 2 + partida->mesa.total);

        if (apuestaActual < apuestaMax) { // Si alguien ya subió
            int diferencia = apuestaMax - apuestaActual;
            if (manoEvaluada.puntuacion >= 3000000) accionDeseada = ACCION_RAISE; // Trío o mejor: quiere re-subir
            else if (manoEvaluada.puntuacion >= 1000000) {
                accionDeseada = (diferencia <= 50) ? ACCION_CALL : ACCION_FOLD; // Paga si no es caro
            } else {
                accionDeseada = ACCION_FOLD; // No tiene nada: se retira
            }
        } else { // Si nadie ha subido
            if (manoEvaluada.puntuacion >= 2000000) accionDeseada = ACCION_RAISE; // Dos Pares o mejor: quiere apostar
            else accionDeseada = ACCION_CHECK; // Menos que eso: pasa
        }
    }

    // === PASO 2: APLICAR RESTRICCIONES ===
    // Ahora que sabemos lo que el bot QUIERE hacer, vemos si PUEDE hacerlo.

    // Si el bot quiere subir, pero ya no puede hacerlo...
    if (accionDeseada == ACCION_RAISE && (jugadorActual->hizoRiseCall || jugadorActual->fichas <= apuestaMax - apuestaActual)) {
        // Se "degrada" su acción. En lugar de subir, igualará la apuesta si es necesario.
        if (apuestaActual < apuestaMax) {
            return ACCION_CALL;
        } else {
            return ACCION_CHECK;
        }
    }
    
    // Si no se aplicó ninguna restricción, el bot realiza la acción que deseaba.
    return accionDeseada;
}


void logicaJugador(Jugador *actual, int *apuestaMax, Partida *partida, int *jugadoresPendientes, int *salir, Jugador *inicio, int *cantidad, Jugador *jug) {
    int opcion;
    mostrarMano(actual->mano);
    printf("\nFichas: %d | Apuesta actual: %d | Apuesta máxima: %d\n\nOPCIONES\n", actual->fichas, actual->apuesta, *apuestaMax);
    if (actual->hizoRiseCall || actual->fichas <= *apuestaMax) { //solo puede hacer check/call o fold porque ya hizo raise o no tiene fichas suficientes
        printf("[1] Call | [2] Fold\n");
        do {
            printf("Elige una opción: ");
            scanf("%d", &opcion);
			getchar();
			
        } while (opcion < 1 || opcion > 2);

        switch (opcion) {
            case 1: // check o call
                checkOrCall(actual, *apuestaMax, partida, jugadoresPendientes);
                break;
            case 2: // Fold
                fold(actual, jugadoresPendientes, partida, salir);
                break;
        }
    } else {
        if (actual->apuesta == *apuestaMax) {
            printf("[1] Check | [2] Raise | [3] Fold\n");
        } else {
            printf("[1] Call [2] Raise [3] Fold\n");
        }
        do {
            printf("Elige una opción: ");
            scanf("%d", &opcion);
			getchar();
        } while (opcion < 1 || opcion > 3);

        switch (opcion) {
            case 1: // check o call
                checkOrCall(actual, *apuestaMax, partida, jugadoresPendientes);
                break;
            case 2: // Raise
                raise(actual, apuestaMax, partida, jugadoresPendientes, inicio, cantidad, jug);
                break;
            case 3: // Fold
                fold(actual, jugadoresPendientes, partida, salir);
                break;
        }
    }
}

void logicaBot(Jugador *actual, int *apuestaMax, Partida *partida, int *jugadoresPendientes, int *salir, Jugador *inicio, int *cantidad, Jugador *jug) {
    printf("\nFichas: %d | Apuesta actual: %d | Apuesta máxima: %d\n\nOPCIONES\n", actual->fichas, actual->apuesta, *apuestaMax);
    int num = (rand() % 10) + 1;

    switch (num) { //mensajes aleatorios para los bots
        case 1:
            printf("%s esta pensando...\n", actual->nombre);
            break;
        case 2:
            printf("%s se puso nervioso...\n", actual->nombre);
            break;
        case 3:
            printf("%s esta contando sus fichas...\n", actual->nombre);
            break;
        case 4:
            printf("%s se cree Brunoid (poker face)...\n", actual->nombre);
            break;
        case 5:
            printf("%s esta sudando...\n", actual->nombre);
            break;
        case 6:
            printf("%s estaba duermiendo...\n", actual->nombre);
            break;
        case 7:
            printf("%s se distrajo...\n", actual->nombre);
            break;
        case 8:
            printf("%s esta rezando...\n", actual->nombre);
            break;
        case 9:
            printf("%s piensa en la colegiatura de sus hijos...\n", actual->nombre);
            break;
        case 10:
            printf("%s no sabe cuanto whisky tomo...\n", actual->nombre);
            break;
    }

    //Sleep(1000); // Simula que el bot "piensa"


    Accion accionBot = tomarDecisiones(partida, actual, *apuestaMax);

    switch (accionBot) {
        case ACCION_CHECK:
        case ACCION_CALL:
            checkOrCall(actual, *apuestaMax, partida, jugadoresPendientes);
            break;
        case ACCION_FOLD:
            fold(actual, jugadoresPendientes, partida, salir);
            break;
        case ACCION_RAISE:
            raise(actual, apuestaMax, partida, jugadoresPendientes, inicio, cantidad, jug);
            break;
    }
}

void rondaDeApuestas(Partida *partida){
    int apuestaMax = obtenerApuestaMaxima(partida->jugadores);
    int jugadoresActivos = contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores));
    int jugadoresPendientes = jugadoresActivos;

    Jugador *jug = clist_first(partida->jugadores);
    Jugador *inicio = jug;
	
    do {//solo pueden actuar una vez los jugadores
        jug->yaActuo = 0;
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
	

    Jugador *actual = clist_first(partida->jugadores);
    inicio = actual;
	while (actual != partida->siguienteApuesta){ //busca al siguiente jugador que puede actuar
		actual = clist_next(partida->jugadores);
		if (actual == inicio) break;
	}

	inicio = actual;
	int salir = 1; // en caso de que todos hagan fold

    while(jugadoresPendientes > 0 && salir){
		printf("JUGADORES PENDIENTES = %d\n", jugadoresPendientes);

		if (strcmp(actual->estado, "Jugando") == 0 && actual->fichas > 0) {

			int cantidad = 0;

			mostrarMesa(partida);

			printf("\nTurno de %s\n", actual->nombre);
			if (actual == partida->jugadorBoton) printf("BOTÓN\n\n");
			if (actual == partida->jugadorCiegaMenor) printf("CIEGA MENOR\n\n");
			if (actual == partida->jugadorCiegaMayor) printf("CIEGA MAYOR\n\n");
			else printf("\n\n");
			
			if (!actual->esBot){ //Si es humano 
				logicaJugador(actual, &apuestaMax, partida, &jugadoresPendientes, &salir, inicio, &cantidad, jug);
			
			}
			else{ //si es bot
				logicaBot(actual, &apuestaMax, partida, &jugadoresPendientes, &salir, inicio, &cantidad, jug);
			}
			
		}
		// Avanza al siguiente jugador circularmente, jugadores que no pueden actuar se saltan
		do {
			actual = clist_next(partida->jugadores);
		} while ((strcmp(actual->estado, "Retirado") == 0 ||actual->fichas == 0 || actual->yaActuo) && actual != inicio);
		presioneTeclaParaContinuar();
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

void iniciarRonda(Partida *partida){
	partida->mesa.ciegaMayor = 8 + 2*(partida->ronda);
	partida->mesa.ciegaMenor = partida->mesa.ciegaMayor/2;
	partida->mesa.bote = 0;
	
	//Ciega Mayor
	if (partida->jugadorCiegaMayor && partida->jugadorCiegaMayor->fichas >= partida->mesa.ciegaMayor){
        partida->jugadorCiegaMayor->fichas -= partida->mesa.ciegaMayor;
		partida->jugadorCiegaMayor->apuesta = partida->mesa.ciegaMayor;
	}
	else if (partida->jugadorCiegaMayor){
		partida->mesa.ciegaMayor = partida->jugadorCiegaMayor->fichas;
		partida->jugadorCiegaMayor->apuesta = partida->mesa.ciegaMayor;
		partida->jugadorCiegaMayor = 0;
	}

	//CIega Menor
    if (partida->jugadorCiegaMenor && partida->jugadorCiegaMenor->fichas >= partida->mesa.ciegaMenor){
        partida->jugadorCiegaMenor->fichas -= partida->mesa.ciegaMenor;
		partida->jugadorCiegaMenor->apuesta = partida->mesa.ciegaMenor;
	}
	else if (partida->jugadorCiegaMenor){
		partida->mesa.ciegaMenor = partida->jugadorCiegaMenor->fichas;
		partida->jugadorCiegaMenor->fichas = 0;
		partida->jugadorCiegaMenor->apuesta = partida->mesa.ciegaMenor;
	}

	//Aumentar el bote
	partida->mesa.bote += partida->mesa.ciegaMayor + partida->mesa.ciegaMenor;
	partida->mesa.total = 0;
	
	barajarCartas(&partida->baraja); // Barajar cartas

	//? Por si quieres un juego personalizado para 3 jugadores (revisar la función funcionTrampa)
	//funcionTrampa(partida);


	repartirCartas(partida);
	rondaDeApuestas(partida);	//primera ronda de apuestas

	if (jugadoresJugando(partida->jugadores, clist_first(partida->jugadores)) == 1){ //si solo queda un jugador activo
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}
	moverIzquierdaBoton(partida);//mover a la izquierda del boton

	crearFlop(partida); //(3 cartas)
	rondaDeApuestas(partida);	//segunda ronda de apuestas

	if (jugadoresJugando(partida->jugadores, clist_first(partida->jugadores)) == 1){ //si solo queda un jugador activo
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}
	moverIzquierdaBoton(partida); //mover a laizquierda del boton

	crearTurn(partida); //(1 carta)
	rondaDeApuestas(partida);	 //tercera ronda de apuestas

	if (jugadoresJugando(partida->jugadores, clist_first(partida->jugadores)) == 1){	 //si solo queda un jugador activo
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}
	moverIzquierdaBoton(partida); //mover a laizquierda del boton

	crearRiver(partida); //(1 carta)
	rondaDeApuestas(partida); //ultima ronda de apuestas
	if (jugadoresJugando(partida->jugadores, clist_first(partida->jugadores)) == 1){ 	//si solo queda un jugador activo
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}
	printf("LLEGAN AL SHOWDOWN\n\n");

	definirGanador(partida);
	limpiarManos(partida); 
	eliminarJugadores(partida->jugadores);
	
	presioneTeclaParaContinuar();
	limpiarPantalla();
	
}

void buscarGanador(Partida *partida){
	Jugador *jug = clist_first(partida->jugadores);
	Jugador *inicio = jug;
	do{
		if(strcmp(jug->estado, "Jugando") == 0 && jug->fichas > 0){
			partida->ganador = jug;
			return;
		}
		jug = clist_next(partida->jugadores);
	} while(jug != inicio);
	partida->ganador = NULL; // No hay ganador
}

void liberarMemoria(Partida *partida)
{
    printf("\nLiberando memoria...\n");
    while (clist_size(partida->jugadores) > 0) 
    {
        // Sacamos el ultimo jugador
        Jugador *jugadorLiberar = clist_popFront(partida->jugadores);

        if (jugadorLiberar != NULL) {
            
            // Si tenemos la mano todavia
            if (jugadorLiberar->mano != NULL) {
                // limpiamos su mano
                list_clean(jugadorLiberar->mano); 

                // y luego liberamos su mano
                free(jugadorLiberar->mano);
            }

            // y liberamos el jugador en si
            free(jugadorLiberar);
        }
    }

    //luego la lista de jugadores como tal se libera tambien
    free(partida->jugadores);
    partida->jugadores = NULL; 

    printf("Se libero la memoria correctamente :D\n\n");

}

void iniciarPartida(){
	char nombre[50];
	int jugadores;
	Partida partida;
	//testeo de elecciones

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
	partida.ronda = 1;
	
	//arrglar repartiR BOTE, fakin bots se llevan todo
	do{	
        int opcion;
        printf("Salir de la partida [9].\n");
        printf("En caso contrario ingrese otro numero: ");
        scanf("%d", &opcion);
        getchar();
        if (opcion == 9){
            mostrarSalida(&partida);
            break;
        }
        limpiarPantalla();
        

		Jugador *jug = clist_first(partida.jugadores);
        Jugador *inicio = jug;

		if (contarJugadoresActivos(partida.jugadores, clist_first(partida.jugadores)) == 1){
            buscarGanador(&partida);
            if (partida.ganador) mostrarGanadorFinal(&partida);
                break;
        }

        printf("=================\nRonda numero : %d\n=================\n\n", partida.ronda);

		printf("\n.----------------------------------------.\n");
        printf("|   TURNOS JUGADORES (arriba -> abajo)   |\n");
        printf("|--------------------|-------------------|\n");
        printf("| Jugador            | Rol               |\n");
        printf("|--------------------|-------------------|\n");

        do {
            char rol[20]; // Variable para guardar la descripción del rol
            if (strcmp(jug->estado, "Jugando") == 0) {
                if (jug == partida.jugadorBoton) {
                    strcpy(rol, "BOTON (Dealer)");
                } else if (jug == partida.jugadorCiegaMayor) {
                    strcpy(rol, "CIEGA MAYOR");
                } else if (jug == partida.jugadorCiegaMenor) {
                    strcpy(rol, "CIEGA MENOR");
                } else if (jug == partida.siguienteApuesta) {
                    strcpy(rol, "Habla Primero");
                } else {
                    strcpy(rol, "-");
                }

                // 2. Imprime la fila completa con formato
                // %-18s significa: "imprime una cadena de texto, alineada a la izquierda,
                // en un espacio de 18 caracteres". Esto crea las columnas.
                printf("| %-18s | %-18s|\n", jug->nombre, rol);
                }
                // 1. Determina el texto del rol antes de imprimir
            

            jug = clist_next(partida.jugadores);
        } while (jug != inicio);

        printf("'----------------------------------------'\n\n");
        printf("\n");
        presioneTeclaParaContinuar();
        limpiarPantalla();
		//Inicia la ronda omg
		iniciarRonda(&partida);

		boton = clist_next(partida.jugadores); // Botón
    	ciegaMenor = clist_next(partida.jugadores); // Ciega menor
    	ciegaMayor = clist_next(partida.jugadores); // Ciega mayor
    	siguienteApuesta = clist_next(partida.jugadores); // Primer jugador en hablar preflop

		partida.jugadorBoton = boton;
		partida.jugadorCiegaMenor = ciegaMenor;
		partida.jugadorCiegaMayor = ciegaMayor;
		partida.siguienteApuesta = siguienteApuesta; //el que parte

		partida.ronda++;

	}while(1);

    presioneTeclaParaContinuar();
    liberarMemoria(&partida);

}

