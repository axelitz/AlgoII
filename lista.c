#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

/* Definición del struct nodo y struct lista.
 */

typedef struct nodo {
	void* dato;
	struct nodo* prox;
} nodo_t;

struct lista {
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

struct lista_iter {
	lista_t* lista;
	nodo_t* anterior;
	nodo_t* actual;
};

/* *****************************************************************
 *               FUNCIONES AUXILIARES PARA NODO
 * *****************************************************************/

nodo_t* nodo_crear(void* valor) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return NULL;
	
	nodo->prox = NULL;
	nodo->dato = valor;
	
	return nodo;
}

void* nodo_destruir(nodo_t* nodo) {
	void* dato = nodo->dato;
	free(nodo);
	return dato;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	
	return lista;
}

bool lista_esta_vacia(const lista_t* lista) {
	return lista->primero == NULL;
}

bool lista_insertar_primero(lista_t* lista, void* dato) {
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	if (lista->primero == NULL) lista->ultimo = nodo_nuevo;
	else nodo_nuevo->prox = lista->primero;
	
	lista->primero = nodo_nuevo;
	lista->largo++;
	
	return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato) {
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	if (lista->ultimo == NULL) lista->primero = nodo_nuevo;
	else lista->ultimo->prox = nodo_nuevo;
	
	lista->ultimo = nodo_nuevo;
	lista->largo++;
	
	return true;
}

void* lista_borrar_primero(lista_t* lista) {
	if (lista_esta_vacia(lista)) return NULL;

	nodo_t* aux = lista->primero;
	lista->primero = lista->primero->prox;
	void* valor = nodo_destruir(aux);
	
	lista->largo--;

	return valor;
}

void* lista_ver_primero(const lista_t* lista) {
	return !lista_esta_vacia(lista) ? lista->primero->dato : NULL;
}

void* lista_ver_ultimo(const lista_t* lista) {
	return !lista_esta_vacia(lista) ? lista->ultimo->dato : NULL;
}

size_t lista_largo(const lista_t* lista) {
	return lista->largo;
}

void lista_destruir(lista_t* lista, void (*destruir_dato)(void*)) {
	while (!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato != NULL) destruir_dato(dato);
	}
	free(lista);
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t* lista, bool (*visitar)(void* dato, void* extra), void* extra) {
	nodo_t* nodo = lista->primero;
	while (nodo != NULL && visitar(nodo->dato, extra)) nodo = nodo->prox;
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t* lista_iter_crear(lista_t* lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;
	
	iter->lista = lista;
	iter->anterior = NULL;
	iter->actual = lista->primero;
		
	return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter) {
	if (iter->actual == NULL) return false;
	
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter) {
	return iter->actual == NULL ? NULL : iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter) {
	return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t* iter) {
	free(iter);
}

/*
 * Primitivas de listas junto con iterador
 */

bool lista_iter_insertar(lista_iter_t* iter, void* dato) {
	nodo_t* nuevo = nodo_crear(dato);
	if (nuevo == NULL) return false;
	
	if (iter->anterior == NULL) iter->lista->primero = nuevo;
	else iter->anterior->prox = nuevo;
	
	if (lista_iter_al_final(iter)) iter->lista->ultimo = nuevo;
	
	nuevo->prox = iter->actual;
	iter->actual = nuevo;
	iter->lista->largo++;
	return true;
}

void* lista_iter_borrar(lista_iter_t* iter) {
	if (iter->actual == NULL) return NULL;
	
	nodo_t* eliminar = iter->actual;
	iter->actual = iter->actual->prox;
	
	if (lista_ver_primero(iter->lista) == eliminar->dato) iter->lista->primero = iter->actual;
	else iter->anterior->prox = iter->actual;
	
	if (lista_iter_al_final(iter)) iter->lista->ultimo = iter->anterior;
	
	iter->lista->largo--;

	void* dato = nodo_destruir(eliminar);
	return dato;
}
