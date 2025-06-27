Póker Texas Hold’em en C
📖 Introducción
Este proyecto es una implementación del juego de póker en su modalidad Texas Hold’em, desarrollado íntegramente en el lenguaje de programación C. La propuesta se presenta como una iniciativa destinada a la aplicación de conceptos avanzados de estructuras de datos y algoritmos en el desarrollo de una aplicación de consola interactiva y funcional.

El propósito de este proyecto es simular una partida de póker completa, gestionando múltiples jugadores (humanos y bots controlados por una IA básica), rondas de apuestas dinámicas, la lógica de evaluación de manos y la distribución de premios.

⚙️ Características Principales
El flujo de una partida completa sigue las reglas estándar de Texas Hold'em:

Inicio de Ronda: Se pagan las apuestas ciegas (chica y grande), cuyos valores aumentan a medida que avanza la partida.

Reparto (Pre-Flop): Se baraja el mazo y se reparten dos cartas privadas a cada jugador.

Rondas de Apuestas: Se realizan cuatro rondas de apuestas: Pre-Flop, post-Flop, post-Turn y post-River.

Cartas Comunitarias:

Flop: Se revelan las primeras 3 cartas comunitarias.

Turn: Se revela la cuarta carta comunitaria.

River: Se revela la quinta y última carta comunitaria.

Showdown: Si al final de la última ronda de apuestas quedan dos o más jugadores, se revelan las cartas. El sistema determina la mejor mano de 5 cartas para cada uno y declara al ganador.

Gestión de Jugadores: Los jugadores sin fichas son eliminados. El botón del repartidor (dealer) y las ciegas rotan a la izquierda para la siguiente ronda.

Estructuras de Datos Utilizadas
Jugador: Una struct que almacena el nombre, fichas, estado actual y apuesta del jugador.

Mano del Jugador: Un TDA Lista (List) para gestionar las dos cartas privadas de cada jugador.

Mesa de Jugadores: Un TDA Lista Circular (CList) para manejar los turnos de forma eficiente y continua.

🛠️ Cómo Compilar y Ejecutar
Asegúrate de tener un compilador de C (como GCC) y, opcionalmente, la utilidad make.

Requisitos
Compilador GCC: Puede instalarse en Windows a través de MinGW-w64.

(Opcional) Make: Para usar el Makefile en Windows, se puede instalar a través de Chocolatey (choco install make).

Instrucciones de Compilación
Puedes compilar y ejecutar el proyecto de tres maneras diferentes:

Método 1: Script build.bat (Para Windows)
Crea un archivo llamado build.bat en la raíz del proyecto.

Copia y pega el siguiente contenido en el archivo:

@echo off
echo Compilando el proyecto...
if not exist build mkdir build
gcc src\poker.c src\juegoPoker\*.c src\tdas\*.c -o build\poker.exe
echo Compilacion finalizada.
pause

Ejecuta el script haciendo doble clic en él o escribiendo build.bat en la consola. El ejecutable poker.exe se creará en la carpeta build.

Método 2: Makefile (Recomendado para cualquier SO)
Asegúrate de tener un archivo Makefile en la raíz del proyecto con el siguiente contenido:

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCS = src/poker.c src/juegoPoker/*.c src/tdas/*.c
TARGET_DIR = build
TARGET = $(TARGET_DIR)/poker

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all run clean

Abre una terminal en la raíz del proyecto y utiliza los siguientes comandos:

make o make all para compilar el proyecto.

make run para compilar (si es necesario) y ejecutar el juego.

make clean para eliminar la carpeta build y el ejecutable.

Método 3: Comando Directo en Consola
Abre una terminal en la raíz del proyecto.

Asegúrate de que la carpeta build exista. Si no, créala con mkdir build.

Ejecuta el siguiente comando para compilar:

gcc src/poker.c src/juegoPoker/*.c src/tdas/*.c -o build/poker.exe

Para ejecutar el juego, navega a la carpeta build y corre el ejecutable:

cd build
./poker.exe

🎮 Ejemplo de Uso
A continuación, se muestra el flujo de una partida a través de la consola.

1. Inicio del Juego
Al ejecutar el programa, se presenta el menú principal. Eliges la opción para iniciar una partida, introduces tu nombre y el número de oponentes bots.

=========================================
|  ♠   ♥   ♣   ♦  POKER  ♦   ♣   ♥   ♠  |
=========================================
1) Iniciar Partida
2) Salir
Ingrese su opción: 1

Tu nombre:
Humano
Cuántos jugadores bots van a jugar? (1-9)
2

2. Comienzo de la Ronda
El juego asigna los roles (Botón, Ciega Pequeña, Ciega Grande) y comienza la primera ronda de apuestas.

=============
Ronda numero : 1
=============

JUGADOR = Humano es EMPIEZA
JUGADOR = Bot 1 es BOTON
JUGADOR = Bot 2 es CIEGA MENOR
JUGADOR = Bot 3 es CIEGA MAYOR

MESA ACTUAL:
BOTE: 15
--------------------------------------------------
(Pre-Flop)

Turno de Humano

Tus cartas:
K ♥
A ♥

Fichas: 100 | Apuesta actual: 0 | Apuesta máxima: 10

OPCIONES
[1] Call [2] Raise [3] Fold
Elige una opción:

3. Acciones del Jugador y los Bots
El jugador y los bots realizan sus acciones por turnos. En este ejemplo, el jugador decide subir la apuesta.

Elige una opción: 2
¿Cuánto quieres subir? (mínimo 10): 20
Humano sube la apuesta a 20.
Presione una tecla para continuar...

Luego, es el turno de los bots, quienes reaccionan a la subida.

MESA ACTUAL:
BOTE: 45

Turno de Bot 2
CIEGA MAYOR

Fichas: 90 | Apuesta actual: 10 | Apuesta máxima: 20

OPCIONES
Bot 2 se puso nervioso...
Bot 2 iguala la apuesta.
Presione una tecla para continuar...

4. Cartas Comunitarias y Siguientes Rondas
Una vez finalizada la ronda de apuestas, se muestra el Flop. El juego continúa con más rondas de apuestas después del Turn y el River.

MESA ACTUAL:
BOTE: 55
--------------------------------------------------
FLOP: 5 ♠   K ♦   J ♣

Turno de Bot 2
CIEGA MAYOR

Fichas: 80 | Apuesta actual: 0 | Apuesta máxima: 0
...

5. Final de la Mano
La mano puede terminar de dos maneras:

Todos se retiran (Fold): Si solo queda un jugador, este gana el bote automáticamente.

TODOS LOS JUGADORES SE HAN RETIRADO
EL JUGADOR Humano ha ganado 55 fichas
Presione una tecla para continuar...

Showdown: Si dos o más jugadores llegan al final, muestran sus cartas y el sistema determina al ganador con la mejor mano.

=== SHOWDOWN ===

> Humano: K ♥ A ♥ - Dos Pares (Puntuación: 2001411)
> Bot 3: 7 ♣ 8 ♣ - Par (Puntuación: 1000807)

===RESULTADO===
Ganador: Humano
Gana 150 fichas
Dos Pares

⚠️ Problemas Conocidos
La inteligencia artificial de los bots es funcional pero básica. Está diseñada para tomar decisiones basadas en la fuerza de su mano, pero puede no siempre elegir la jugada más óptima desde una perspectiva estratégica avanzada.

El juego está diseñado para terminales de texto. La visualización puede variar ligeramente dependiendo de la configuración de la consola utilizada.