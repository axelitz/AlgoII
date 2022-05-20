#include "heap.h"
#include "testing.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LARGO_VOLUMEN 10000

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

int cmpstr(const void* a, const void* b) {
	return strcmp((char*)a, (char*)b);
}

int cmpint(const void* a, const void* b) {
	if (*(int*)a == *(int*)b) return 0;
	return *(int*)a > *(int*)b ? 1 : -1;
}

void cambiar_valores (size_t* a, size_t* b) {
	size_t temp = *a;
	*a = *b;
	*b = temp;
}

void permutacion_aleatoria (size_t arr[], size_t n) {
	srand ((unsigned) time(NULL));
	for (size_t i = n-1; i > 0; i--) {
		size_t j = rand() % (i+1);
		cambiar_valores(&arr[i], &arr[j]);
	}
}

size_t** crear_array_aleatorio_de_punteros_sizet(size_t largo) {
	size_t aleatorio[largo];
	for (size_t i = 0; i < largo; i++) aleatorio[i] = i+1;
	permutacion_aleatoria(aleatorio, largo);
	size_t** aleatorio_punteros = malloc (largo*sizeof(size_t*));
	for (size_t i = 0; i < largo; i++) {
		aleatorio_punteros[i] = malloc(sizeof(size_t));
		*aleatorio_punteros[i] = aleatorio[i];
	}
	return aleatorio_punteros;
}

bool chequear_ordenamiento(size_t** arreglo, size_t largo) {
	for (size_t i = 0; i < largo; i++) if (*arreglo[i] != i+1) return false;
	return true;
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_heap_vacio() {
	printf("\nINICIO DE PRUEBAS HEAP VACIO\n");
	heap_t* heap = heap_crear(NULL);

	print_test("Prueba heap crear heap vacio", heap);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap ver máximo, es NULL, no hay elementos", heap_ver_max(heap) == NULL);
	print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));
	print_test("Prueba heap desencolar, es NULL, no hay elementos para desencolar", heap_desencolar(heap) == NULL);

	heap_destruir(heap, NULL);
}

static void prueba_heap_encolar_desencolar() {
	printf("\nINICIO DE PRUEBAS DE ENCOLADO Y DESENCOLADO EN HEAP\n");
	heap_t* heap = heap_crear(cmpstr);

	char* dato1 = "guau";
	char* dato2 = "miau";
	char* dato3 = "mu";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba heap encolar dato1", heap_encolar(heap, dato1));
	print_test("Prueba heap encolar dato2", heap_encolar(heap, dato2));
	print_test("Prueba heap encolar dato3", heap_encolar(heap, dato3));
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	while(!heap_esta_vacio(heap)) heap_desencolar(heap);
	print_test("Prueba heap desencolar hasta que esté vacío", heap_esta_vacio(heap));
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap desencolar, es NULL, no hay elementos para desencolar", heap_desencolar(heap) == NULL);
	print_test("Prueba heap obtener maximo es NULL", heap_ver_max(heap) == NULL);

	heap_destruir(heap, NULL);
}

static void prueba_heap_encolar_strings() {
	printf("\nINICIO DE PRUEBAS DE ENCOLADO DE STRINGS EN HEAP\n");
	heap_t* heap = heap_crear(cmpstr);

	char* dato1 = "guau";
	char* dato2 = "miau";
	char* dato3 = "mu";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba heap encolar dato1", heap_encolar(heap, dato1));
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap obtener maximo es dato1", heap_ver_max(heap) == dato1);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));
	print_test("Prueba heap desencolar, es dato1", heap_desencolar(heap) == dato1);
	print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));

	/* Inserta otros 2 valores y se desencola uno solo (el otro se destruye con el heap) */
	print_test("Prueba heap encolar dato2", heap_encolar(heap, dato2));
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap obtener maximo es dato2", heap_ver_max(heap) == dato2);

	print_test("Prueba heap encolar dato3", heap_encolar(heap, dato3));
	print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
	char* dato_mayor = cmpstr(dato2, dato3) > 0 ? dato2 : dato3;
	print_test("Prueba heap obtener maximo es el mayor de ambos datos", heap_ver_max(heap) == dato_mayor);
	print_test("Prueba heap desencolar, es el dato mayor", heap_desencolar(heap) == dato_mayor);
	print_test("Prueba heap obtener maximo es el otro dato", heap_ver_max(heap) == (dato_mayor == dato2 ? dato3 : dato2));

	heap_destruir(heap, NULL);
}

static void prueba_heap_encolar_numeros_y_destruir() {
	printf("\nINICIO DE PRUEBAS DE ENCOLADO DE NUMEROS EN HEAP\n");
	heap_t* heap = heap_crear(cmpint);

	int* dato1 = malloc(sizeof(int));
	*dato1 = 1;
	int* dato2 = malloc(sizeof(int));
	*dato2 = 2;
	int* dato3 = malloc(sizeof(int));
	*dato3 = 0;
	
	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba heap encolar dato1", heap_encolar(heap, dato1));
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap obtener maximo es dato1", heap_ver_max(heap) == dato1);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));

	/* Inserta otros 2 valores */
	print_test("Prueba heap encolar dato2", heap_encolar(heap, dato2));
	int* dato_mayor1 = cmpint(dato1, dato2) > 0 ? dato1 : dato2;
	print_test("Prueba heap obtener maximo es el mayor de ambos datos", heap_ver_max(heap) == dato_mayor1);

	print_test("Prueba heap encolar dato3", heap_encolar(heap, dato3));
	int* dato_mayor2 = cmpint(dato_mayor1, dato3) > 0 ? dato_mayor1 : dato3;
	print_test("Prueba heap obtener maximo es el mayor de los 3 datos", heap_ver_max(heap) == dato_mayor2);
	print_test("Prueba heap destruccion, se liberan punteros creados mediante funcion de destruccion", true);

	heap_destruir(heap, free);
}

static void prueba_heapsort() {
	printf("\nINICIO DE PRUEBAS DE HEAPSORT\n");
	
	size_t largo = 10;
	size_t** aleatorio = crear_array_aleatorio_de_punteros_sizet(largo);
	heap_sort((void**)aleatorio, largo, cmpint);
	print_test("Prueba heapsort ordena correctamente", chequear_ordenamiento(aleatorio, largo));
	for (size_t i = 0; i < largo; i++) free(aleatorio[i]);
	free(aleatorio);
}

static void prueba_crear_heap_inicializado() {
	printf("\nINICIO DE PRUEBAS DE HEAP CON VALORES DE INICIALIZACIÓN\n");

	size_t largo = 10;
	size_t** aleatorio = crear_array_aleatorio_de_punteros_sizet(largo);
	heap_t* heap = heap_crear_arr((void**)aleatorio, largo, cmpint);
	print_test("Prueba heap la cantidad de elementos es igual a la del arreglo de inicialización", heap_cantidad(heap) == largo);
	for (size_t i = 0; i < largo; i++) aleatorio[i] = heap_desencolar(heap);
	bool ok = true;
	for (size_t i = 0; i < largo-1; i++) if (*aleatorio[i] < *aleatorio[i+1]) {
		ok = false;
		break;
	}
	print_test("Prueba heap los elementos se desencolan según su prioridad", ok);
	heap_destruir(heap, NULL);

	heap = heap_crear_arr((void**)aleatorio, largo, cmpint);
	heap_destruir(heap, free);
	free(aleatorio);
}

static void prueba_heap_volumen(size_t cant_elementos) {
	printf("\nINICIO DE PRUEBAS DE VOLUMEN EN HEAP\n");
	heap_t* heap = heap_crear(cmpint);
	
	/* Encolado de muchos elementos */
	size_t** aleatorio = crear_array_aleatorio_de_punteros_sizet(cant_elementos);
	if (aleatorio == NULL) {
		print_test("No se pudo realizar la prueba de volumen debido a que no hay memoria dinamica para un vector de elementos", aleatorio != NULL);
		return;
	}
	
	/* Encolado de elementos + Prueba de que el primero siempre corresponde al primer elemento agregado */
	bool ok = true;
	for (size_t i = 0; i < cant_elementos; i++) heap_encolar(heap, aleatorio[i]);
	print_test("El valor máximo del heap es el valor máximo de todos los elementos agregados", *(int*)heap_ver_max(heap) == cant_elementos);
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == cant_elementos);

	/* Desencolado de elementos + Prueba de que se respeta la prioridad elemento a elemento desencolado */
	ok = true;
	for (size_t i = 0; i < cant_elementos; i++) {
		size_t* elemento = (size_t*)heap_desencolar(heap);
		size_t valor = *elemento;
		free(elemento);
		if (valor != cant_elementos-i) {
			ok = false;
			break;
		}
	}
	print_test("Prueba heap los elementos se desencolan según su prioridad", ok);
	print_test("Los elementos se desencolan hasta llegar al vacio", heap_esta_vacio(heap));
	
	/* Destruyo vector y heap */
	heap_destruir(heap, free);
	free(aleatorio);
}

/* ******************************************************************
 *                      CONJUNTO DE PRUEBAS
 * *****************************************************************/

void pruebas_heap_estudiante(void) {
	prueba_crear_heap_vacio();
	prueba_heap_encolar_desencolar();
	prueba_heap_encolar_strings();
	prueba_heap_encolar_numeros_y_destruir();
	prueba_heapsort();
	prueba_crear_heap_inicializado();
	prueba_heap_volumen(LARGO_VOLUMEN);
}
