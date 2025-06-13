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

gcc tdas/*.c poker.c -Wno-unused-result -o poker
./poker