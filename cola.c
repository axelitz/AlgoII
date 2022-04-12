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
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t *cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL) return NULL;
	
	cola->primero = NULL;
	cola->ultimo = NULL;
	
	return cola;
}

// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
	nodo_t* nodo = cola->primero;
	nodo_t* aux;
	while (nodo != NULL) {
		if (destruir_dato != NULL) destruir_dato(nodo->dato);
		aux = nodo;
		nodo = nodo->prox;
		free(aux);
	}
	free(cola);
}

// Devuelve verdadero si la cola no tiene elementos encolados, false en caso contrario.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

nodo_t* crear_nodo(void* valor) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return NULL;
	
	nodo->prox = NULL;
	nodo->dato = valor;
	
	return nodo;
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void *valor){
	nodo_t* nodo_nuevo = crear_nodo(valor);
	if (nodo_nuevo == NULL) return false;

	if (cola->primero == NULL) cola->primero = nodo_nuevo;
	else cola->ultimo->prox = nodo_nuevo;
	
	cola->ultimo = nodo_nuevo;
	return true;
}

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void *cola_ver_primero(const cola_t *cola){
	return cola->primero == NULL ? NULL : cola->primero->dato;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void *cola_desencolar(cola_t *cola){
	if (cola->primero == NULL) return NULL;
	if (cola->primero->prox == NULL) cola->ultimo = NULL;
	
	void* valor = cola->primero->dato;
	nodo_t* desencolado = cola->primero;
	cola->primero = cola->primero->prox;
	free(desencolado);
	
	return valor;
}
