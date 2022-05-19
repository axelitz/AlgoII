#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void* elementos[], size_t cant, cmp_func_t cmp) {
}

struct heap {
	void **datos;
	size_t tam;
	size_t cant;
	cmp_func_t cmp;
};

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t* heap_crear(cmp_func_t cmp) {
	return NULL;
}

/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/
heap_t* heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp) {
	return NULL;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t* heap, void (*destruir_elemento)(void* e)) {
}

size_t heap_cantidad(const heap_t* heap) {
	return heap->cant;
}

bool heap_esta_vacio(const heap_t* heap) {
	return heap->cant == 0;
}

/* Agrega un elemento al heap.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t *heap, void* elem) {
	return false;
}

void* heap_ver_max(const heap_t* heap) {
	return heap_esta_vacio(heap) ? NULL : heap->datos[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void* heap_desencolar(heap_t* heap) {
	return NULL; 
}
