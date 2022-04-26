#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

/* Definición del struct nodo y struct cola.
 */

typedef struct nodo {
    void *dato;
    struct nodo *prox;
} nodo_t;

struct cola {
    nodo_t *primero;
    nodo_t *ultimo;
};

/* *****************************************************************
 *               FUNCIONES AUXILIARES PARA NODO
 * *****************************************************************/

nodo_t* crear_nodo(void* valor) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return NULL;
	
	nodo->prox = NULL;
	nodo->dato = valor;
	
	return nodo;
}

void* destruir_primer_nodo(cola_t* cola) {
	void* valor = cola->primero->dato;
	nodo_t* aux = cola->primero;
	cola->primero = cola->primero->prox;
	free(aux);
	return valor;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL) return NULL;
	
	cola->primero = NULL;
	cola->ultimo = NULL;
	
	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
	while (cola->primero != NULL) {
		void* dato = destruir_primer_nodo(cola);
		if (destruir_dato != NULL) destruir_dato(dato);
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){
	nodo_t* nodo_nuevo = crear_nodo(valor);
	if (nodo_nuevo == NULL) return false;

	if (cola->primero == NULL) cola->primero = nodo_nuevo;
	else cola->ultimo->prox = nodo_nuevo;
	
	cola->ultimo = nodo_nuevo;
	return true;
}

void *cola_ver_primero(const cola_t *cola){
	return !cola_esta_vacia(cola) ? cola->primero->dato : NULL;
}

void *cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;
	if (cola->primero->prox == NULL) cola->ultimo = NULL;
	
	void* valor = destruir_primer_nodo(cola);
	return valor;
}
