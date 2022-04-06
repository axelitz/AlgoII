#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
	
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL){
		return NULL;
	}
	
	pila->capacidad = 10;
	
	pila->datos = malloc(pila->capacidad*sizeof(void*));
	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	
	pila->cantidad = 0;
	return pila;
}

void pila_destruir(pila_t *pila){
	pila->capacidad = 0;
	pila->cantidad = 0;
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){

	if (pila->cantidad == 0) {
		return true;
	}
	
	return false;
}

bool pila_apilar(pila_t *pila, void *valor){

	if (pila->cantidad == pila->capacidad){
		
		int factor_crecimiento = 2;
		void** datos = realloc(pila->datos,factor_crecimiento*pila->capacidad*sizeof(void*));
		
		if (datos == NULL) {
			return false;
		}
		
		pila->datos = datos;
		pila->capacidad = factor_crecimiento*pila->capacidad;
	}
	
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void *pila_ver_tope(const pila_t *pila){

	if (pila_esta_vacia(pila)) {
		return NULL;
	}
	
	return pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){

	if (pila_esta_vacia(pila)) {
		return NULL;
	}
	
	void* desapilado = pila->datos[pila->cantidad-1];
	pila->cantidad--;
	return desapilado;
}
