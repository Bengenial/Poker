#include "clist.h"
#include <stdlib.h>

typedef struct CNode {
    void *data;
    struct CNode *next;
    struct CNode *prev;
} CNode;

struct CList {
    CNode *head;
    CNode *tail;
    CNode *current;
    int size;
};

CList *clist_create() {
    CList *L = (CList *)malloc(sizeof(CList));
    if (!L) return NULL;
    L->head = NULL;
    L->tail = NULL;
    L->current = NULL;
    L->size = 0;
    return L;
}

void clist_pushFront(CList *L, void *data) {
    CNode *n = (CNode *)malloc(sizeof(CNode));
    if (!n) return;
    n->data = data;
    if (!L->head) {
        n->next = n;
        n->prev = n;
        L->head = n;
        L->tail = n;
    } else {
        n->next = L->head;
        n->prev = L->tail;
        L->head->prev = n;
        L->tail->next = n;
        L->head = n;
    }
    L->size++;
}

void clist_pushBack(CList *L, void *data) {
    CNode *n = (CNode *)malloc(sizeof(CNode));
    if (!n) return;
    n->data = data;
    if (!L->tail) {
        n->next = n;
        n->prev = n;
        L->head = n;
        L->tail = n;
    } else {
        n->next = L->head;
        n->prev = L->tail;
        L->tail->next = n;
        L->head->prev = n;
        L->tail = n;
    }
    L->size++;
}

void *clist_first(CList *L) {
    if (!L || !L->head) return NULL;
    L->current = L->head;
    return L->current->data;
}

void *clist_last(CList *L) {
    if (!L || !L->tail) return NULL;
    L->current = L->tail;
    return L->current->data;
}

void *clist_next(CList *L) {
    if (!L || !L->current) return NULL;
    L->current = L->current->next;
    return L->current->data;
}

void *clist_prev(CList *L) {
    if (!L || !L->current) return NULL;
    L->current = L->current->prev;
    return L->current->data;
}

void *clist_popFront(CList *L) {
    if (!L || !L->head) return NULL;
    CNode *n = L->head;
    void *data = n->data;
    if (L->head == L->tail) {
        L->head = NULL;
        L->tail = NULL;
    } else {
        L->head = n->next;
        L->head->prev = L->tail;
        L->tail->next = L->head;
    }
    free(n);
    L->size--;
    return data;
}

void *clist_popBack(CList *L) {
    if (!L || !L->tail) return NULL;
    CNode *n = L->tail;
    void *data = n->data;
    if (L->head == L->tail) {
        L->head = NULL;
        L->tail = NULL;
    } else {
        L->tail = n->prev;
        L->tail->next = L->head;
        L->head->prev = L->tail;
    }
    free(n);
    L->size--;
    return data;
}

void clist_clean(CList *L) {
    while (L && L->size > 0) clist_popFront(L);
}

int clist_size(CList *L) {
    if (!L) return 0;
    return L->size;
}