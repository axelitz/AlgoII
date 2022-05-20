#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TAM_INICIAL 10
#define FACTOR_CREC 2
#define FACTOR_DEC 0.5f
#define MIN_PROP 4
#define RAIZ 0

struct heap {
	void** datos;
	size_t tam;
	size_t cant;
	cmp_func_t cmp;
};

/* *****************************************************************
 *                 	FUNCIONES AUXILIARES
 * *****************************************************************/

bool heap_redimensionar(heap_t* heap, float factor_crecimiento) {
	size_t valor_ajuste = (size_t)(factor_crecimiento*(float)heap->tam);

	if (valor_ajuste >= TAM_INICIAL) {
		void** datos = realloc(heap->datos,valor_ajuste*sizeof(void*));
		if (datos == NULL) return false;

		heap->datos = datos;
		heap->tam = valor_ajuste;
	}
	return true;
}

void swap_en_arreglo(void** datos, size_t index_1, size_t index_2) {
	void* aux = datos[index_1];
	datos[index_1] = datos[index_2];
	datos[index_2] = aux;
}

void heap_upheap(void** datos, size_t cant, size_t index_elem, cmp_func_t cmp) {
	if (index_elem == RAIZ || index_elem >= cant) return;

	size_t index_padre = (index_elem-1)/2;
	if (cmp(datos[index_padre], datos[index_elem]) < 0) {
		swap_en_arreglo(datos, index_padre, index_elem);
		heap_upheap(datos, cant, index_padre, cmp);
	}
}

void heap_downheap(void** datos, size_t cant, size_t index_elem, cmp_func_t cmp) {
	if (cant < 2*index_elem + 2) return;

	size_t index_hijo = cant < 2*index_elem + 3 || cmp(datos[2*index_elem + 1], datos[2*index_elem + 2]) > 0 ? 2*index_elem + 1 : 2*index_elem + 2;
	if (cmp(datos[index_hijo], datos[index_elem]) > 0) {
		swap_en_arreglo(datos, index_hijo, index_elem);
		heap_downheap(datos, cant, index_hijo, cmp);
	}
}

void heapify(void** arreglo, size_t n, cmp_func_t cmp) {
	for (size_t i = n; i > RAIZ; i--) heap_downheap(arreglo, n, i-1, cmp);
}

void swap_de_raiz_y_downheap(void** arreglo, size_t cant, cmp_func_t cmp) {
	swap_en_arreglo(arreglo, RAIZ, cant-1);
	heap_downheap(arreglo, cant-1, RAIZ, cmp);
}

/* *****************************************************************
 *                     PRIMITIVAS DEL HEAP
 * *****************************************************************/

void heap_sort(void* elementos[], size_t cant, cmp_func_t cmp) {
	heapify(elementos, cant, cmp);
	for (size_t i = 0; i < cant; i++) swap_de_raiz_y_downheap(elementos, cant-i, cmp);
}

heap_t* heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (heap == NULL) return NULL;
	
	heap->tam = TAM_INICIAL;
	heap->datos = malloc(heap->tam*sizeof(void*));
	if (heap->datos == NULL) {
		free(heap);
		return NULL;
	}
	heap->cant = 0;
	heap->cmp = cmp;
	
	return heap;
}

heap_t* heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = heap_crear(cmp);
	if (heap == NULL) return NULL;
	
	while (heap->tam < n && heap_redimensionar(heap, FACTOR_CREC));
	if (heap->tam < n) {
		heap_destruir(heap, NULL);
		return NULL;
	} 
	for (size_t i = 0; i < n; i++) heap->datos[i] = arreglo[i];
	heap->cant = n;

	heapify(heap->datos, heap->cant, heap->cmp);
	return heap;
}

void heap_destruir(heap_t* heap, void (*destruir_elemento)(void* e)) {
	if (destruir_elemento != NULL) for (size_t i = RAIZ; i < heap->cant; i++) destruir_elemento(heap->datos[i]);
	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t* heap) {
	return heap->cant;
}

bool heap_esta_vacio(const heap_t* heap) {
	return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void* elem) {
	if (heap->tam == heap->cant && !heap_redimensionar(heap, FACTOR_CREC)) return false;

	heap->datos[heap->cant] = elem;
	heap_upheap(heap->datos, heap->cant+1, heap->cant, heap->cmp);	
	heap->cant++;
	return true;
}

void* heap_ver_max(const heap_t* heap) {
	return !heap_esta_vacio(heap) ? heap->datos[RAIZ] : NULL;
}

void* heap_desencolar(heap_t* heap) {
	if (heap_esta_vacio(heap)) return NULL;

	if (heap->cant*MIN_PROP <= heap->tam/* && heap->tam > TAM_INICIAL*/) heap_redimensionar(heap, FACTOR_DEC);

	swap_de_raiz_y_downheap(heap->datos, heap->cant, heap->cmp);

	void* desencolado = heap->datos[heap->cant-1];
	heap->cant--;
	return desencolado;
}
