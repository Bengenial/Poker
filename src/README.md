# pokerTexasHoldem

# Póker Texas Hold’em

Este es un juego de póker en su modalidad Texas Hold’em, utilizando el lenguaje de programación C. La propuesta se presenta
como una iniciativa destinada a la aplicación de conceptos avanzados de estructuras de datos y
algoritmos en el desarrollo de una aplicación computacional interactiva y funcional. El propósito
de este proyecto es simular una partida de póker completa, gestionando múltiples jugadores (tanto
humanos como controlados por el ordenador), rondas de apuestas, la lógica de las manos y la
distribución de premios

## Descripción

Para enriquecer la experiencia de juego, esta aplicación de póker Texas Hold’em cuenta con dos
modalidades de interacción. Se mantiene la interfaz de consola basada en texto, que permite un
control directo y rápido del juego contra oponentes de IA mediante comandos sencillos. A esta se
suma una nueva interfaz gráfica de usuario (GUI), diseñada para ser más intuitiva y visualmente
atractiva. Esta GUI facilita la inmersión en el juego, ya que representa la mesa, las cartas y las
acciones a través de botones y elementos gráficos, y hace que la aplicación sea más accesible y
fácil de usar para el jugador.
Cada Partida sigue el flujo de:

 a. Iniciar una nueva ronda y pagar las apuestas ciegas (blinds).
 b. Barajar el mazo y repartir dos cartas a cada jugador.
 c. Primera ronda de apuestas (pre-flop).
 d. Mostrar el Flop (las primeras 3 cartas comunitarias).
 e. Segunda ronda de apuestas.
 f. Mostrar el Turn (la cuarta carta comunitaria).
 g. Tercera ronda de apuestas.
 h. Mostrar el River (la quinta y última carta comunitaria).
 i. Ronda de apuestas final.
 j. Showdown: determinar el ganador, mostrar las manos y repartir el bote.
 k. Verificar y eliminar jugadores sin fichas.
 l. Mover el botón del dealer y las ciegas para la siguiente ronda.

 La información del jugador (nombre, fichas, estado) se almacena en una estructura, su mano se gestiona con un TDA List, y el conjunto
 de jugadores en la mesa se gestiona con un TDA CList. La lista circular es perfecta para
 manejar turnos en póker, y en cada turno cada Jugador / Bot puede hacer las siguentes acciones:

– Pasar (Check): No apostar, cediendo el turno pero continuando en la ronda (solo si no
 hay apuestas previas).
 – Igualar (Call): Apostar la misma cantidad que el último jugador que subió.– Subir (Raise): Aumentar la apuesta actual, forzando a los demás a igualar, subir de
 nuevo o retirarse.
 – Retirarse (Fold): Abandonar la mano actual, perdiendo cualquier ficha apostada en la
 ronda.
 - RAISE//////////////////////////

También el usuario podrá elegir que nombre usará en el juego:

- Ejemplo de uso : El jugador elige Iniciar partida
- Ingresa su nombre Ej: "Usuario"

## Cómo compilar y ejecutar

### Requisitos (PENDIENTE)

- Tener instalado **Visual Studio Code** o cualquier editor compatible con C.
- Tener un compilador de C como **GCC** (por ejemplo, a través de **MinGW** en Windows).
- Asegurarse de tener los archivos fuente necesarios, incluyendo los de los TDA (`CList.h`, `List.h`, y sus respectivos `.c`)./////////

### Pasos para compilar y ejecutar (PENDIENTE)

1. Descargar el proyecto completo.
2. Descomprimir el contenido en una carpeta local.
3. Abrir la carpeta del proyecto en **Visual Studio Code**.
4. Abrir un terminal integrado en el editor.
5. Compilar el programa con el siguiente comando:

- ./build/poker.exe ?
```bash
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
```

- ./build.bat ?
6. Ejecuta el programa
```bash
./tarea2
```

## Comportamiento de las funcionalidades


A continuación se describen las funcionalidades implementadas, indicando su comportamiento esperado:

### Iniciar partida:

Configura el juego desde cero, solicitando el número de oponentes, creando
los jugadores, asignando fichas y preparando la baraja y la mesa.

### Iniciar ronda:

Se ejecuta al comienzo de cada mano, barajando el mazo, repartiendo dos
cartas a cada jugador y reiniciando las apuestas.

### Acciones del jugador:

Durante su turno, un jugador puede realizar una de las siguientes
acciones:
– Pasar (Check): No apostar, cediendo el turno pero continuando en la ronda (solo si no
 hay apuestas previas).
 – Igualar (Call): Apostar la misma cantidad que el último jugador que subió.– Subir (Raise): Aumentar la apuesta actual, forzando a los demás a igualar, subir de
 nuevo o retirarse.
 – Retirarse (Fold): Abandonar la mano actual, perdiendo cualquier ficha apostada en la
 ronda.

### Tomar decisiones de la IA:

Los jugadores no humanos podrán elegir una acción en base a
sus cartas y las cartas comunitarias.

### Gestión de cartas comunitarias:

El juego revela automáticamente las cartas en la mesa
en tres etapas: Flop (3 cartas), Turn (1 carta) y River (1 carta), con una ronda de apuestas
después de cada una.

### Ronda de apuestas:

Administra el ciclo de turnos hasta que todos los jugadores activos
hayan igualado la apuesta más alta o se hayan retirado.

### Determinar ganador:

Al final de la última ronda de apuestas (o si solo queda un jugador),
el sistema evalúa la mejor mano de 5 cartas para cada jugador (usando sus 2 cartas y las 5 de
la mesa) y declara al ganador.

### Gestión del bote y fichas:

Acumula todas las apuestas en un bote central y lo entrega al
ganador. Si un jugador se queda sin fichas, es eliminado de la partida

### Salir del programa :

Finaliza la ejecución del programa de forma segura, liberando memoria.

---
 (ANTIGUAMENTE FUNCIONALIDADES ADICIONALES)
### Interfaz Gráfica de Usuario (GUI)
 La GUI ofrece una representación visual y compacta de la partida con los siguientes elementos
 clave:
 • Jugadores: Muestra el nombre, fichas y cartas de cada participante (visibles para el humano,
 ocultas para los bots).
 • Mesa Central: Despliega las cartas comunitarias (Flop, Turn, River) y el pozo total de
 apuestas.
 • Panel de Acciones: Contiene los botones para que el jugador ejecute sus movimientos
 (Igualar, Subir, etc.)

### Problemas conocidos: (PENDIENTE)
- El tiempo de carga para todos los datos del archivo csv puede ser un poco larga, para ello está comentada una condicion que solo carga las primeras 10000 canciones en la función `cargarCanciones()`
- Algunas canciones están repetidas, sólo difieren en su id
- No se realiza validacion completa de la entrada: Al inicio del menú principal si se ingresa una cadena con el primer caracter válido `opciones 1,2,3,4,5,6,7,8`, se almacena el resto de la cadena para la siguiente instrucción, por lo que puede realizar tareas que no se desean o acelerar el ingreso de datos (no recomendado).



## Ejemplo de uso (LO HAREMOS SIN ESTO O CON ESTO, POR LO DE LA GUI ?????)

"========================================"
"           ♠️  ♥️  Poker  ♦️  ♣️"
"========================================"
"       ♠       ♥       ♣        ♦       "
"1) Iniciar Partida                      "
"2) Salir                                "

### Pasos (LO HAREMOS SIN ESTO O CON ESTO, POR LO DE LA GUI ?????)

1. **Iniciar Partida**  
   El sistema carga las cartas desde un archivo CSV.

   **-------------------------------------------------------**  
   `Ingrese su opción:  1`
   **-------------------------------------------------------**

    El sistema carga las cartas y luego inicia las rondas, luego el jugador debe ingresar su nombre y La cantidad de bots (MÁX 9).


---

2. **Rondas**  
    Se ejecuta al comienzo de cada mano, barajando el mazo, repartiendo dos
    cartas a cada jugador y reiniciando las apuestas (Ejemplo con 3 Bots:)

   **-------------------------------------------------------**  
   `JUGADOR = A es EMPIEZA`
    `JUGADOR = Bot 1 es BOTON`
    `JUGADOR = Bot 2 es CIEGA MENOR`
    `JUGADOR = Bot 3 es CIEGA MAYOR`
    `JUGADORES PENDIENTES = 4`
    `MESA ACTUAL:`
    `BOTE: 15`


    `Turno de Usuario`


    `Tus cartas:`
    `9 ♥`
    `10 ♥`

    `Fichas: 100 | Apuesta actual: 0 | Apuesta máxima: 10`

    `OPCIONES`
    `[1] Call [2] Raise [3] Fold`
    `Elige una opción:`

   **-------------------------------------------------------**
    – Pasar (Check): No apostar, cediendo el turno pero continuando en la ronda (solo si no
    hay apuestas previas).
    – Igualar (Call): Apostar la misma cantidad que el último jugador que subió.– Subir (Raise): Aumentar la apuesta actual, forzando a los demás a igualar, subir de
    nuevo o retirarse.
    – Retirarse (Fold): Abandonar la mano actual, perdiendo cualquier ficha apostada en la
    ronda.

---

3. **Acciones**  
   Un ejemplo donde el jugador eligió Call :

   **-------------------------------------------------------**  
   
    `Fichas: 100 | Apuesta actual: 0 | Apuesta máxima: 10`
    `Fichas: 100 | Apuesta actual: 0 | Apuesta máxima: 10`

    `OPCIONES`
    `[1] Call [2] Raise [3] Fold`
    `Elige una opción: 1`
    `Usuario iguala la apuesta.`
    `Presione una tecla para continuar...`
   **-------------------------------------------------------**

   El sistema muestra todas las acciones del Jugador y después de los bots.
   **-------------------------------------------------------**
    `JUGADORES PENDIENTES = 3`
    `MESA ACTUAL:`
    `BOTE: 25`


    `Turno de Bot 1`
    `BOTÓN`




    `Fichas: 100 | Apuesta actual: 0 | Apuesta máxima: 10`

    `OPCIONES`
    `Bot 1 no sabe cuanto whisky tomo...`
    `Bot 1 se retira.`
    `Presione una tecla para continuar...`
   **-------------------------------------------------------**

Al pasar por las acciones de todos los Bots, se terminó la primera ronda y se muestran las cartas comunitarias, esta vez se eligió raise.

   **-------------------------------------------------------**
    `MESA ACTUAL:`
    `BOTE: 25`

    `FLOP`
    `Carta [1]: 10 ♠`
    `Carta [2]: J ♠`
    `Carta [3]: 4 ♠`

    `Turno de Usuario`


    `Tus cartas:`
    `9 ♥`
    `10 ♥`

    `Fichas: 90 | Apuesta actual: 0 | Apuesta máxima: 0`

    `OPCIONES`
    `[1] Check | [2] Raise | [3] Fold`
    `Elige una opción: 2`
    `¿Cuánto quieres subir? (mínimo 1): 10`
    `A sube la apuesta a 10.`
    `Presione una tecla para continuar...`
   **-------------------------------------------------------**
---

4. **Condiciónes de victoria**
   Al final de la última ronda de apuestas (o si solo queda un jugador),
   el sistema evalúa la mejor mano de 5 cartas para cada jugador (usando sus 2 cartas y las 5 de
   la mesa) y declara al ganador
   
   Acumula todas las apuestas en un bote central y lo entrega al
   ganador. Si un jugador se queda sin fichas, es eliminado de la partida.

   En este ejemplo todos los Bots decidieron retirarse, y termina ganando el Jugador : 

   **-------------------------------------------------------**  
   `TODOS LOS JUGADORES SE HAN RETIRADO`
    `EL JUGADOR Usuario ha ganado 35 fichas`
    `Presione una tecla para continuar...`
   **-------------------------------------------------------**


5. **Salir de la aplicación**
   El sistema termina liberando la memoria de los Jugadores y cartas creadas

   **-------------------------------------------------------**
   `Ingrese su opción: 2`

   `Saliendo...`
   **-------------------------------------------------------**

# pokerTexasHoldem

extracto de wikipedia:
**poker**
En el juego casual, el derecho a repartir una mano suele rotar entre los jugadores y está marcado por una ficha llamada botón de repartir (o buck). En un casino, un crupier de la casa maneja las cartas de cada mano, pero el botón (normalmente un disco de plástico blanco) gira en el sentido de las agujas del reloj entre los jugadores para indicar un crupier nominal que determine el orden de las apuestas. Las cartas se reparten en el sentido de las agujas del reloj alrededor de la mesa de póquer, de una en una.

Normalmente, uno o más jugadores tienen que hacer apuestas forzadas, normalmente un ante o una apuesta ciega (a veces ambas). El crupier baraja las cartas, el jugador de la silla de su derecha corta, y el crupier reparte el número apropiado de cartas a los jugadores de una en una, empezando por el jugador de su izquierda. Las cartas pueden repartirse boca arriba o boca abajo, dependiendo de la variante de póquer que se esté jugando. Tras el reparto inicial, comienza la primera de lo que pueden ser varias rondas de apuestas. Entre rondas, las manos de los jugadores evolucionan de alguna manera, a menudo repartiendo cartas adicionales o sustituyendo las cartas repartidas anteriormente. Al final de cada ronda, todas las apuestas se reúnen en el bote central.

En cualquier momento durante una ronda de apuestas, si un jugador apuesta, ningún oponente decide igualar la apuesta y todos los oponentes se retiran, la mano termina inmediatamente, el apostante se lleva el bote, no es necesario mostrar cartas y comienza la siguiente mano. Esto es lo que hace posible el farol o engaño. El farol es una característica principal del póquer, que lo distingue de otros juegos de apuestas y de otros juegos que utilizan clasificación de manos de póquer.

Al final de la última ronda de apuestas, si queda más de un jugador, los jugadores revelan sus cartas previamente ocultas y evalúan sus manos. El jugador con la mejor mano según la variante de póquer que se esté jugando gana el pozo. Una mano de póquer consta de cinco cartas; en las variantes en las que un jugador dispone de más de cinco cartas, sólo cuenta la mejor combinación de cinco cartas. Existen 10 tipos diferentes de manos de póquer, como la escalera de color y el cuádruple de cartas.

**flop**
En las distintas variantes de póquer en que se usan naipes comunitarios, como es el caso del Texas hold 'em o el Omaha hold 'em, se llama flop a los tres naipes que se ubican boca arriba en la mesa; esta es la segunda de las cuatro rondas que se encuentran en una mano de cualquiera de estas modalidades.

En esta ronda se sacan al centro de la mesa las tres primeras cartas comunes boca arriba y vuelve a comenzar la acción empezando por la ciega pequeña, el jugador a la izquierda del botón.

Los naipes que conforman el flop deben sacarse siempre de la parte superior del mazo y para poder evitar estafas el primero de los naipes que quedó en la cima del mazo mientras duraba la primera rueda de apuestas se elimina sin enseñarlo y de esta manera se toman los que forman el flop (a esto se le llama "quemar" la primera carta).

Para poder verse el flop es preciso que más de un jugador permanezca en la mano después de la primera ronda de apuestas, ya que si los jugadores abandonan la mano, el jugador que permanece automáticamente gana la mano, sin la necesidad de enseñar las tres primeras cartas comunes.

En la situación en que varios jugadores permanezcan después de la primera ronda de apuestas (preflop), para que los jugadores puedan "ver el flop", es obligatorio que estos hayan apostado la misma cantidad al pot.

**turn**
En el caso de que dos o más jugadores permanezcan en la mano después del flop, se saca otra carta comunitaria más boca arriba, llamada turn, no sin antes volver a "quemar" una carta. Una vez la carta esté visible, volverá a comenzar la acción el jugador que esté a la izquierda del botón. Al acabar esta nueva ronda de apuestas, en caso de que siga habiendo dos o más jugadores en la jugada, se destapará, después de volver a "quemar" una carta, la última carta comunitaria, llamada river.

**river**
El river es sin duda la carta más determinante, ya que es la última que se muestra. Si los jugadores no han ligado ya su jugada, esta es su última oportunidad. El primer jugador en hablar vuelve a ser el jugador que esté a la izquierda del botón. Una vez terminada esta ronda si se han igualado las apuestas en caso de haberlas, tendrá que enseñar su mano el jugador que ha apostado, y el que igualó la jugada podrá elegir si mostrarlas para llevarse la cantidad en el bote, o no enseñar las cartas que tenía.

**clasificación de manos**
Clasificación de las manos
Artículo principal: Probabilidad en el póquer
Top	Nombre en español	Nombre en inglés	Descripción	Ejemplo	Combinaciones Posibles	Probabilidad
1	Escalera real o flor imperial	Royal flush	Cinco cartas seguidas del mismo palo del 10 al as.		4 de 2 598 960	0,000154%
2	Escalera de color	Straight flush	Cinco cartas consecutivas del mismo palo.		36 de 2 598 960	0,001385%
3	Póker	Four of a kind o Quad	Cuatro cartas iguales en su valor.		624 de 2 598 960	0,024%
4	Full	Full House	Tres cartas iguales en su valor (trío), más otras dos iguales en su valor (pareja).		3744 de 2 598 960	0,1440576%
5	Color	Flush	Cinco cartas del mismo palo, sin ser consecutivas.		5108 de 2 598 960	0,1965%
6	Escalera	Straight	Cinco cartas formando una escalera de números.		10 200 de 2 598 960	0,3924%
7	Trío	Three of a kind o Set	Tres cartas iguales de valor.		54 912 de 2 598 960	2,1113%
8	Doble pareja	Two pair o Pocket	Dos pares de cartas del mismo número (par y par).		123 552 de 2 598 960	4,759%
9	Pareja	One pair	Dos cartas iguales de número (y tres diferentes).		1 098 240 de 2 598 960	42,257%
10	Carta alta	High card	Gana quien tiene la carta más alta de todas.		1 302 540 de 2 598 960	50,1177%

Todos deben tener dos cartas al inicio.

make
make run
make clear
