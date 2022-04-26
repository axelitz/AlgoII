#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

/* Definición del struct nodo y struct lista.
 */

typedef struct nodo {
    void *dato;
    struct nodo *prox;
} nodo_t;

struct lista {
    nodo_t *primero;
    nodo_t *ultimo;
    size_t largo;
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

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	
	return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
	return lista->primero == NULL;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	if (lista->primero == NULL) lista->ultimo = nodo_nuevo;
	else nodo_nuevo->prox = lista->primero;
	
	lista->primero = nodo_nuevo;
	lista->largo++;
	
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL) return false;

	if (lista->ultimo == NULL) lista->primero = nodo_nuevo;
	else lista->ultimo->prox = nodo_nuevo;
	
	lista->ultimo = nodo_nuevo;
	lista->largo++;
	
	return true;
}

void *lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;

	void* valor = lista->primero->dato;
	nodo_t* aux = lista->primero;
	lista->primero = lista->primero->prox;
	free(aux);
	
	lista->largo--;

	return valor;
}

void *lista_ver_primero(const lista_t *lista) {
	return !lista_esta_vacia(lista) ? lista->primero->dato : NULL;
}

void* lista_ver_ultimo(const lista_t* lista) {
	return !lista_esta_vacia(lista) ? lista->ultimo->dato : NULL;
}

size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
	while (!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato != NULL) destruir_dato(dato);
	}
	free(lista);
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/
 
 
// Iterador interno de la lista.
// Recorre la lista y para cada elemento de la misma llama a la función visitar, pasándole por parámetro el elemento de la lista
// (dato) y el parámetro extra. El parámetro extra sirve para que la función visitar pueda recibir información adicional. Puede ser NULL.
// La función visitar debe devolver true si se debe seguir iterando, false para cortar la iteración. No puede agregar ni quitar elementos
// a la lista).
// Pre: la lista fue creada.
// Post: se llamó a la función visitar una vez por cada elemento de la lista, en orden.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra) {
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/
 
// Obtiene un iterador de la lista
// Pre: la lista fue creada
// Post: se devuelve un nuevo iterador
lista_iter_t *lista_iter_crear(lista_t *lista) {
	return NULL;
}

// Avanza una posición en la lista
// Pre: el iterador fue creado
// Post: se avanzó la posición actual en el iterador. Devuelve false si ya
// está al final de la lista
bool lista_iter_avanzar(lista_iter_t *iter) {
	return true;
}

// Almacena en dato el valor que se encuentra en la posición actual
// Pre: el iterador fue creado
// Post: se almacenó el dato actual en dato. Devuelve false si está al final
// de la lista
void *lista_iter_ver_actual(const lista_iter_t *iter) {
	return NULL;
}

// Verifica si ya está al final de la lista
// Pre: el iterador fue creado
bool lista_iter_al_final(const lista_iter_t *iter) {
	return true;
}

// Destruye el iterador de la lista
void lista_iter_destruir(lista_iter_t *iter) {
}

/*
 * Primitivas de listas junto con iterador
 */

// Agrega un elemento en la posición actual
// Pre: el iterador fue creado
// Post: se insertó un nuevo nodo antes de la posición actual, quedando este
// nuevo nodo como posición actual
bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	return true;
}

// Elimina el elemento de la posición actual
// Pre: el iterador fue creado
// Post: se eliminó el nodo que se encontraba en la posición actual indicada por el
// iterador. Devuelve NULL si el iterador está al final de la lista
void *lista_iter_borrar(lista_iter_t *iter) {
	return NULL;
}
