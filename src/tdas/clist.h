#ifndef CLIST_H
#define CLIST_H

typedef struct CList CList;

// Crea una nueva lista circular
CList *clist_create();

// Inserta al inicio
void clist_pushFront(CList *L, void *data);

// Inserta al final
void clist_pushBack(CList *L, void *data);

// Elimina y retorna el primer elemento
void *clist_popFront(CList *L);

// Elimina y retorna el último elemento
void *clist_popBack(CList *L);

// Retorna el primer elemento y lo deja como actual
void *clist_first(CList *L);

// Retorna el último elemento y lo deja como actual
void *clist_last(CList *L);

// Avanza al siguiente (circular)
void *clist_next(CList *L);

// Retrocede al anterior (circular)
void *clist_prev(CList *L);

// Retorna el tamaño de la lista
int clist_size(CList *L);

// Limpia la lista y libera memoria
void clist_clean(CList *L);

// Esta función devuelve el dato del nodo actual (current) sin mover el puntero.
void *clist_getCurrent(CList *L);

#endif