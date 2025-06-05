#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>





int main() {
    // Estado inicial del puzzle

    char opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");
        
        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");
    
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
    
        switch (opcion) {
        case '1':
          //dfs(estado_inicial);
          break;
        case '2':
          //bfs(estado_inicial);
          break;
        case '3':
          //best_first(estado_inicial);
          break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != '4');

  return 0;
}
