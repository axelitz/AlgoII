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

#define TAM_INICIAL 10
#define FACTOR_CREC 2
#define FACTOR_DEC 0.5f

pila_t *pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL){
		return NULL;
	}
	
	pila->capacidad = TAM_INICIAL;
	
	pila->datos = malloc(pila->capacidad*sizeof(void*));
	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	
	pila->cantidad = 0;
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_redimensionar(pila_t* pila, float factor_crecimiento) {
	size_t valor_ajuste = (size_t)(factor_crecimiento*(float)pila->capacidad);

	if (valor_ajuste >= TAM_INICIAL) {
		void** datos = realloc(pila->datos,valor_ajuste*sizeof(void*));

		if (datos == NULL) return false;

		pila->datos = datos;
		pila->capacidad = valor_ajuste;
	}
	return true;
}

bool pila_apilar(pila_t *pila, void *valor){
	if (pila->cantidad == pila->capacidad && !pila_redimensionar(pila, FACTOR_CREC)) return false;
	
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void *pila_ver_tope(const pila_t *pila){
	return pila_esta_vacia(pila) ? NULL : pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila)) {
		return NULL;
	}
	
	if (pila->cantidad*4 <= pila->capacidad && pila->capacidad > TAM_INICIAL) {
		pila_redimensionar(pila, FACTOR_DEC);
	}
	
	void* desapilado = pila->datos[pila->cantidad-1];
	pila->cantidad--;
	return desapilado;
}
