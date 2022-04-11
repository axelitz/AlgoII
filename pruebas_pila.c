#include "pila.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

int** crear_vector(int cant) {
	/* Creo vector de tamaño cant */
	int** vector = malloc(cant*sizeof(int*));
	if (vector == NULL) {
		return NULL;
	}
	
	for(int i = 0; i < cant; i++) {
		vector[i] = malloc(sizeof(int*));
		*vector[i] = i;
	}
	return vector;
}

void destruir_vector(int** vector, int cant) {
	/* Destruyo el vector creado */
	for(int i = 0; i < cant; i++) {
		free(vector[i]);
	}
	free(vector);
}

// 1
static void prueba_crear_destruir(void) {
	/* Prueba de pila creada correctamente */
	pila_t* pila = pila_crear();
	print_test("La pila se pudo crear", pila != NULL);
	
	/* Destruyo pila */
	pila_destruir(pila);

}

// 2
static void prueba_invariante(void) {
	/* Pŕueba más detallada del invariante en la pila (analizando un puntero en particular) */
	pila_t* pila = pila_crear();
	pila_apilar(pila, NULL);
	
	/* Puntero a observar en la prueba */
	void* puntero = malloc(sizeof(void*));
	pila_apilar(pila, puntero);
	
	/* Apilo y desapilo un vector */
	int cant_elementos = 5;
	int** elementos = crear_vector(cant_elementos);
	
	int i = 0;
	while (i < cant_elementos) {
		pila_apilar(pila,elementos[i]);
		i++;
	}

	while (i > 0) {
		i--;
		pila_desapilar(pila);
	}
	
	/* Prueba del invariante luego de apilado y desapilado */
	print_test("La pila se apila y desapila correctamente, obteniendo el tope deseado", pila_ver_tope(pila) == puntero);
	
	/* Destruyo vector, puntero y pila */
	destruir_vector(elementos,cant_elementos);
	free(puntero);
	pila_destruir(pila);
}

// 3
static void prueba_apilar_desapilar(void) {
	pila_t* pila = pila_crear();
	
	/* Apilado con más elementos que la capacidad inicial */
	int cant_elementos = 10000;
	int** elementos = crear_vector(cant_elementos);
	
	/* Apilado de elementos + Prueba de que el tope siempre corresponde al último elemento agregado */
	bool apilado = true;
	int i = 0;
	while (i < cant_elementos) {
		pila_apilar(pila,elementos[i]);
		if (pila_ver_tope(pila) != elementos[i]) {
			apilado = false;
		} 
		i++;
	}
	print_test("El tope siempre es el último elemento apilado", apilado == true);

	/* Desapilado de elementos + Prueba de que se respeta el invariante elemento a elemento desapilado */
	apilado = true;
	while (i > 0) {
		i--;
		int* desapilado = pila_desapilar(pila);
		if (desapilado != elementos[i]) {
			apilado = false;
		} 
	}
	print_test("Se cumple el invariante al desapilar", apilado == true);

	/* Prueba de pila vacía al desapilar todos los elementos */
	apilado = true;
	if(!pila_esta_vacia(pila)) {
		apilado = false;
	}
	print_test("Los elementos se desapilan hasta llegar al vacio", apilado == true);
	
	/* Destruyo vector y pila */
	destruir_vector(elementos,cant_elementos);
	pila_destruir(pila);
}

// 4
static void prueba_apilar_null(void) {
	/* Prueba de apilar elemento NULL */
	pila_t* pila = pila_crear();
	bool apilar_null = pila_apilar(pila, NULL);
	if (pila_ver_tope(pila) != NULL) {
		apilar_null = false;
	}
	print_test("El elemento NULL es apilable", apilar_null == true);
	
	/* Destruyo pila */
	pila_destruir(pila);
}

// 5 + 8
static void prueba_apilar_vaciar_recien_creada(void) {
	pila_t* pila = pila_crear();
	
	/* Apilado con más elementos que la capacidad inicial */
	int cant_elementos = 10000;
	int** elementos = crear_vector(cant_elementos);
	
	/* Apilado de elementos */
	int i = 0;
	while (i < cant_elementos) {
		pila_apilar(pila,elementos[i]);
		i++;
	}
	
	/* Desapilado de elementos */
	while (i > 0) {
		i--;
		pila_desapilar(pila);
	}
	
	/* Prueba de que pila desapilada se comporta como una pila vacía (no se ve tope ni se puede desapilar) */
	bool desapilado = true;
	if (pila_ver_tope(pila) != NULL || pila_desapilar(pila) != NULL) {
		desapilado = false;
	}
	print_test("Al ser desapilada, la pila se comporta como recién creada.", desapilado == true);
	
	/* Destruyo vector y pila */
	destruir_vector(elementos,cant_elementos);
	pila_destruir(pila);
}

// 6
static void prueba_pila_recien_creada_desapilar_ver_tope(void) {
	pila_t* pila = pila_crear();
	bool recien_creada = true;
	/* Prueba de ver tope vacío y de no poder desapilar al recién crear la pila */
	if (pila_ver_tope(pila) != NULL || pila_desapilar(pila) != NULL) {
		recien_creada = false;
	}
	print_test("La pila al recién crearse no puede verse el tope ni desapilar", recien_creada == true);
	
	/* Destruyo pila */
	pila_destruir(pila);
}

// 7
static void prueba_pila_recien_creada_vacia(void) {
	pila_t* pila = pila_crear();
	/* Prueba de pila vacía al crearse */
	print_test("La pila al recién crearse está vacía", pila_esta_vacia(pila) == true);
	
	/* Destruyo pila */
	pila_destruir(pila);
}

void pruebas_pila_estudiante() {
    /* Ejecuta todas las pruebas. */
    prueba_crear_destruir(); // 1
    prueba_invariante(); // 2
    prueba_apilar_desapilar(); // 3
    prueba_apilar_null(); // 4
    prueba_apilar_vaciar_recien_creada(); // 5 + 8
    prueba_pila_recien_creada_desapilar_ver_tope(); // 6
    prueba_pila_recien_creada_vacia(); // 7
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
