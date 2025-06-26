#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "../tdas/extra.h"

#include "motorPrincipal.h"

//Del poker
#include "logicaCartas.h"
#include "accionesJugador.h"
#include "visualizacion.h"

//Funciones Auxiliares
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

void funcionTrampa(Partida *partida){
	Carta carta;
	//1 JUG
	strcpy(carta.valor,"A");
	strcpy(carta.color,"diamantes");
	partida->baraja.cartas[0] = carta;

	strcpy(carta.valor,"K");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[1] = carta;

	//2 JUG
	strcpy(carta.valor,"9");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[2] = carta;

	strcpy(carta.valor,"8");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[3] = carta;

	//3 JUG
	strcpy(carta.valor,"4");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[4] = carta;

	strcpy(carta.valor,"K");
	strcpy(carta.color,"picas");
	partida->baraja.cartas[5] = carta;

	//MESA
	strcpy(carta.valor,"J");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[6] = carta;

	strcpy(carta.valor,"10");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[7] = carta;

	strcpy(carta.valor,"Q");
	strcpy(carta.color,"corazones");
	partida->baraja.cartas[8] = carta;

	strcpy(carta.valor,"7");
	strcpy(carta.color,"tréboles");
	partida->baraja.cartas[9] = carta;

	strcpy(carta.valor,"2");
	strcpy(carta.color,"picas");
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
	Sleep(300);
	mostrarMesa(partida->mesa);
	Sleep(300);
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
        printf("%s: %d fichas\n", jug->nombre, jug->fichas);
        jug = clist_next(partida->jugadores);
    } while (jug != inicio);
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

/*Accion tomarDecisiones(ManoEvaluada manoActual, int apuestaActual, int apuestaMax)
{	
	//Tener en cuenta que la IA es lo más basico, osea va a ser predesible (por el momento...)
	if(apuestaActual < apuestaMax){

		if(manoActual.puntuacion < 1000000) //numero por definirse, pero es un ejemplo
		{ //pesima mano
			return ACCION_FOLD; //en este caso si aumentan la apuesta, me retiro (fold)
		}
		else{
			return ACCION_CALL;
		}
	}
	else{
		if(manoActual.puntuacion < 30000000)
		{ //mano decente
			return ACCION_CHECK;
		}
		else{ //si tiene una muy buena mano
			return ACCION_RAISE; //va a aumentar la apuesta
		}
	}
}*/

// VERSIÓN MEJORADA DE LA IA CON LÓGICA PRE-FLOP Y POST-FLOP
static Accion tomarDecisiones(Partida *partida, Jugador *jugadorActual, int apuestaMax)
{
    int apuestaActual = jugadorActual->apuesta;
    ManoEvaluada manoEvaluada;

    // --- LÓGICA PRE-FLOP (cuando no hay cartas en la mesa) ---
    if (partida->mesa.total == 0) {
        Carta *c1 = list_first(jugadorActual->mano);
        Carta *c2 = list_next(jugadorActual->mano);
        int val1 = obtenerValorCarta(c1->valor);
        int val2 = obtenerValorCarta(c2->valor);

        // Nivel 1: Manos Premium (Pares Altos, AKs, AQs) -> Siempre sube o resube
        if ((val1 == val2 && val1 >= 11) || (val1 >= 13 && val2 >= 12 && strcmp(c1->color, c2->color) == 0)) {
            return ACCION_RAISE;
        }
        // Nivel 2: Manos Buenas (Pares Medios, Conectores del mismo palo) -> Paga apuestas normales
        else if ((val1 == val2 && val1 >= 7) || (val1 >= 10 && val2 >= 9 && strcmp(c1->color, c2->color) == 0)) {
            if (apuestaActual < apuestaMax) return ACCION_CALL; // Paga si hay subida
            else return ACCION_CHECK; // Pasa si no la hay
        }
        // Nivel 3: Manos Especulativas -> Paga solo si la apuesta es barata (la ciega)
        else {
            if (apuestaActual < apuestaMax) {
                if (apuestaMax <= 10) return ACCION_CALL; // Paga la ciega
                else return ACCION_FOLD; // Se retira ante una subida de verdad
            } else {
                return ACCION_CHECK;
            }
        }
    }
    // --- LÓGICA POST-FLOP (cuando ya hay cartas en la mesa) ---
    else {
        Carta cartasCombinadas[7];
        combinarCartasJugador(jugadorActual->mano, partida->mesa, cartasCombinadas);
        manoEvaluada = evaluarMano(cartasCombinadas, 2 + partida->mesa.total);

        // La lógica que ya habíamos mejorado
        if (apuestaActual < apuestaMax) { // Alguien subió
            int diferencia = apuestaMax - apuestaActual;
            if (manoEvaluada.puntuacion >= 3000000) return ACCION_RAISE; // Trío o mejor: resube
            else if (manoEvaluada.puntuacion >= 1000000) { // Par o Dos Pares
                if (diferencia <= 50) return ACCION_CALL; // Paga si no es caro
                else return ACCION_FOLD;
            } else {
                return ACCION_FOLD; // Nada: retírate
            }
        } else { // Nadie ha subido
            if (manoEvaluada.puntuacion >= 2000000) return ACCION_RAISE; // Dos Pares o mejor: apuesta
            else return ACCION_CHECK; // Peor que eso: pasa
        }
    }
}

void logicaJugador(Jugador *actual, int apuestaMax, Partida *partida, int *jugadoresPendientes, int *salir, Jugador *inicio, int *cantidad, Jugador *jug) {
    int opcion;
    mostrarMano(actual->mano);
    printf("\nFichas: %d | Apuesta actual: %d | Apuesta máxima: %d\n\nOPCIONES\n", actual->fichas, actual->apuesta, apuestaMax);
    if (actual->hizoRiseCall || actual->fichas <= apuestaMax) {
        printf("[1] Call | [2] Fold\n");
        do {
            printf("Elige una opción: ");
            if (scanf("%d", &opcion) != 1) {
                while (getchar() != '\n');
                opcion = 0;
            }
        } while (opcion < 1 || opcion > 2);

        switch (opcion) {
            case 1: // check o call
                checkOrCall(actual, apuestaMax, partida, jugadoresPendientes);
                break;
            case 2: // Fold
                fold(actual, jugadoresPendientes, partida, salir);
                break;
        }
    } else {
        if (actual->apuesta == apuestaMax) {
            printf("[1] Check | [2] Raise | [3] Fold\n");
        } else {
            printf("[1] Call [2] Raise [3] Fold\n");
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
                checkOrCall(actual, apuestaMax, partida, jugadoresPendientes);
                break;
            case 2: // Raise
                raise(actual, &apuestaMax, partida, jugadoresPendientes, inicio, cantidad, jug);
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

    switch (num) {
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

    Sleep(1500); // Simula que el bot "piensa"

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

void rondaDeApuestas(Partida *partida){ //reconocer si es humano o no
    int apuestaMax = obtenerApuestaMaxima(partida->jugadores);
    int jugadoresActivos = contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores));
    int jugadoresPendientes = jugadoresActivos;

    Jugador *jug = clist_first(partida->jugadores);
    Jugador *inicio = jug;
	
    do {
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
			
			if (!actual->esBot){ //Si es humano 
				logicaJugador(actual, apuestaMax, partida, &jugadoresPendientes, &salir, inicio, &cantidad, jug);
			
			}
			else{ //si es bot
				logicaBot(actual, &apuestaMax, partida, &jugadoresPendientes, &salir, inicio, &cantidad, jug);
			}
			
		}
		// Avanza al siguiente jugador circularmente
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

	//funcionTrampa(partida);
	repartirCartas(partida);
	rondaDeApuestas(partida);//agregar IArand dsp
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}	//foldearon
	moverIzquierdaBoton(partida);//mover a la izquierda del boton

	crearFlop(partida); //(3 cartas)
	rondaDeApuestas(partida);
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}	//foldearon
	//mover a laizquierda del boton
	moverIzquierdaBoton(partida);

	crearTurn(partida); //(1 carta)
	rondaDeApuestas(partida);
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}	//foldearon
	//mover a laizquierda del boton
	moverIzquierdaBoton(partida);

	crearRiver(partida); //(1 carta)
	rondaDeApuestas(partida); //final
	if (contarJugadoresActivos(partida->jugadores, clist_first(partida->jugadores)) == 1){
		mostrarGandorFold(partida);
		limpiarManos(partida);
		return;
	}	//foldearon
	printf("LLEGAN AL SHOWDOWN\n\n");

	definirGanador(partida);
	limpiarManos(partida); 

	eliminarJugadores(partida->jugadores);
	
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
	partida.ronda = 0;
	
	
	Jugador *jug = siguienteApuesta;
    Jugador *inicio = jug;

    

	//antes iniciarRonda(partida,IArand);
	do{	

		//1.-En cada ronda aumentar las ciegas (menor y mayor) 
		//2.-Si el jugador, mostrar directamente el ganador

		printf("\n\n=============\nRonda numero : %d\n=============\n\n", partida.ronda);
		do {
			printf("JUGADOR = %s es ", jug->nombre);
			if(jug == partida.jugadorBoton) printf("BOTON\n");
			else if (jug == partida.jugadorCiegaMayor) printf("CIEGA MAYOR\n");
			else if (jug == partida.jugadorCiegaMenor) printf("CIEGA MENOR\n");
			else if (jug == partida.siguienteApuesta) printf("EMPIEZA\n");
			else printf("NORMAL\n");

			jug = clist_next(partida.jugadores);
		} while (jug != inicio);

		//Inicia la ronda omg
		iniciarRonda(&partida);
		boton = clist_next(partida.jugadores); // Botón
    	ciegaMenor = clist_next(partida.jugadores); // Ciega menor
    	ciegaMayor = clist_next(partida.jugadores); // Ciega mayor
    	siguienteApuesta = clist_next(partida.jugadores); // Primer jugador en hablar preflop

		partida.jugadorBoton = boton;
		partida.jugadorCiegaMenor = ciegaMenor;
		partida.jugadorCiegaMayor = ciegaMayor;
		partida.siguienteApuesta = siguienteApuesta; //parte

		partida.ronda++;
	}while(partida.numJugadores > 1);

}

