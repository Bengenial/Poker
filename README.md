
# ♠️ Póker Texas Hold’em en C

## 📖 Introducción

Este proyecto es una implementación del juego de póker en su modalidad Texas Hold’em, desarrollado íntegramente en el lenguaje de programación C. La propuesta se presenta como una iniciativa destinada a la aplicación de conceptos avanzados de estructuras de datos y algoritmos en el desarrollo de una aplicación de consola interactiva y funcional.

El propósito de este proyecto es simular una partida de póker completa, gestionando múltiples jugadores (humanos y bots controlados por una IA básica), rondas de apuestas dinámicas, la lógica de evaluación de manos y la distribución de premios.

El juego de Póker Texas Hold'em sigue reglas específicas que puedes revisar [aquí](https://es.wikipedia.org/wiki/Texas_hold_%27em).

---

## ⚙️ Características Principales

El flujo de una partida completa sigue las reglas estándar de Texas Hold'em:

- **Inicio de Ronda:** Se pagan las apuestas ciegas (chica y grande), cuyos valores aumentan a medida que avanza la partida.
- **Reparto (Pre-Flop):** Se baraja el mazo y se reparten dos cartas privadas a cada jugador.
- **Rondas de Apuestas:** Se realizan cuatro rondas de apuestas: Pre-Flop, post-Flop, post-Turn y post-River.
- **Cartas Comunitarias:**
  - **Flop:** Se revelan las primeras 3 cartas comunitarias.
  - **Turn:** Se revela la cuarta carta comunitaria.
  - **River:** Se revela la quinta y última carta comunitaria.
- **Showdown:** Si al final de la última ronda de apuestas quedan dos o más jugadores, se revelan las cartas. El sistema determina la mejor mano de 5 cartas para cada uno y declara al ganador.
- **Gestión de Jugadores:** Los jugadores sin fichas son eliminados. El botón del repartidor (dealer) y las ciegas rotan a la izquierda para la siguiente ronda.

---

## 🧱 Estructuras de Datos Utilizadas

- **Partida** Una `struct` que almacena a los jugadores, la mesa y la baraja.
- **Jugador:** Una `struct` que almacena el nombre, fichas, estado actual y apuesta del jugador.
- **Mano del Jugador:** Un TDA Lista (`List`) para gestionar las dos cartas privadas de cada jugador.
- **Mesa de Jugadores:** Un TDA Lista Circular (`CList`) para manejar los turnos de forma eficiente y continua.

---

## 🛠️ Cómo Compilar y Ejecutar

Asegúrate de tener un compilador de C (como GCC) y, opcionalmente, la utilidad `make`.

### Requisitos

- **Compilador GCC:** Puede instalarse en Windows a través de MinGW-w64.
- **(Opcional) Make:** Para usar el Makefile en Windows, se puede instalar a través de Chocolatey (`choco install make`).

### Instrucciones de Compilación

Puedes compilar y ejecutar el proyecto de tres maneras diferentes:

#### 🔹 Método 1: Script `build.bat` (Para Windows) (recomendado para una ventana completa)

- Ejecuta el script haciendo doble clic en él o escribiendo `build.bat` en la consola.  
El ejecutable `poker.exe` se creará en la carpeta `build`.
```bash
.\build.bat
```

---

#### 🔹 Método 2: Makefile (Recomendado para cualquier SO)

Asegúrate de tener un archivo `Makefile` en la raíz del proyecto

- `make`: compila el proyecto.
- `make run`: compila (si es necesario) y ejecuta el juego.
- `make clean`: elimina la carpeta `build` y el ejecutable.

#### 🔹 Método 3: Comando Directo en Consola

1. Abre una terminal en la raíz del proyecto.
2. Asegúrate de que la carpeta `build` exista: `mkdir build`
3. Compila con:

```bash
gcc src/poker.c src/juegoPoker/*.c src/tdas/*.c -o build/poker.exe
```

4. Ejecuta el juego:

```bash
cd build
./poker.exe
```

---

## 🎮 Ejemplo de Uso

### 1. Inicio del Juego

```text
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
```

---

### 2. Comienzo de la Ronda

```text
================
Ronda numero : 1
================


.----------------------------------------.
|   TURNOS JUGADORES (arriba -> abajo)   |
|--------------------|-------------------|
| Jugador            | Rol               |
|--------------------|-------------------|
| Benja              | Habla Primero     |
| Bot 1              | BOTON (Dealer)    |
| Bot 2              | CIEGA MENOR       |
| Bot 3              | CIEGA MAYOR       |
'----------------------------------------'


Presione una tecla para continuar...
```

```text
MESA ACTUAL:
BOTE: 15
--------------------------------------------------
PRE-FLOP
CIEGA MAYOR

Turno de Usuario

Tus cartas:
K ♥
A ♥

Fichas: 100 | Apuesta actual: 0 | Apuesta máxima: 10

OPCIONES
[1] Call [2] Raise [3] Fold
Elige una opción:
```

---

### 3. Acciones del Jugador y los Bots

```text
Elige una opción: 2
¿Cuánto quieres subir? (mínimo 10): 20
Humano sube la apuesta a 20.
Presione una tecla para continuar...

MESA ACTUAL:
BOTE: 40

.---------------------.
|PRE-FLOP             |
|CIEGA MAYOR: 10      |
.---------------------.

Turno de Bot 3
CIEGA MAYOR


Fichas: 90 | Apuesta actual: 10 | Apuesta máxima: 20

OPCIONES
Bot 3 esta sudando...
Bot 3 iguala la apuesta
Presione una tecla para continuar...
```

---

### 4. Cartas Comunitarias y Siguientes Rondas

```text
MESA ACTUAL:
BOTE: 55

.---------------------.
|        FLOP         |
.---------------------.
Carta [1]: 5 ♠
Carta [2]: K ♥
Carta [3]: A ♦

Turno de Bot 2
CIEGA MENOR

Fichas: 90 | Apuesta actual: 0 | Apuesta máxima: 0
...
```

---

### 5. Final de la Mano

- **Todos se retiran (Fold):**

```text
TODOS LOS JUGADORES SE HAN RETIRADO
EL JUGADOR Humano ha ganado 55 fichas
Presione una tecla para continuar...
```

- **Showdown:**

```text
=== SHOWDOWN ===

> Humano: K ♥ A ♥ - Dos Pares (Puntuación: 2001411)
> Bot 3: 7 ♣ 8 ♣ - Par (Puntuación: 1000807)

=== RESULTADO ===
Ganador: Humano
Gana 150 fichas
Dos Pares
```

---

## ⚠️ Problemas Conocidos

- La inteligencia artificial de los bots es funcional pero básica. Está diseñada para tomar decisiones basadas en la fuerza de su mano, pero puede no siempre elegir la jugada más óptima desde una perspectiva estratégica avanzada.
- El juego está diseñado para terminales de texto. La visualización puede variar ligeramente dependiendo de la configuración de la consola utilizada.
- Problemas con el current, al pasar a un jugador como puntero se pierde el curren de la lista, por lo que para solucionarlo se debe recorrer la lista nuevamente. (posible problemas con los turnos)
- Al repartir el bote no se hace un side-pot, acorde a las reglas del poker Texas Hold'em