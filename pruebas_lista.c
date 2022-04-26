#include "lista.h"
#include "cola.h"
#include "pila.h"
#include "vector.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

void destruir_vector(int** vector, size_t cant) {
	/* Destruyo el vector creado */
	for(int i = 0; i < cant; i++) free(vector[i]);
	free(vector);
}

int** crear_vector(size_t cant) {
	/* Creo vector de tamaño cant */
	int** vector = malloc(cant*sizeof(int*));
	if (vector == NULL) return NULL;
	
	for(int i = 0; i < cant; i++) {
		vector[i] = malloc(sizeof(int*));
		if (vector[i] == NULL) {
			destruir_vector(vector, i-1);
			return NULL;
		}
		*vector[i] = i;
	}
	return vector;
}

void destruir_cola(void* cola) {
	cola_destruir(cola, NULL);
}

void destruir_pila(void* pila) {
	pila_destruir(pila);
}

void destruir_vector_TDA(void* vector) {
	vector_destruir(vector);
}

// 1
static void prueba_crear_destruir(void) {
	/* Prueba de lista creada correctamente */
	lista_t* lista = lista_crear();
	print_test("La lista se pudo crear", lista != NULL);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_largo_lista(void) {
	lista_t* lista = lista_crear();
	
	/* Prueba de correcta longitud de vector */
	size_t longitud[] = {0, 10, 100};
	size_t long_vector = 3;
	
	bool correcto_largo = true;
	for (size_t i = 0; i < long_vector; i++) {
		int** vector = crear_vector(longitud[i]);
		/* Prueba de longitud de lista al agregar elementos */ 
		for (int j = 0; j < longitud[i]; j++) lista_insertar_primero(lista,vector[j]);
		if (lista_largo(lista) != longitud[i]) correcto_largo = false;
		
		/* Prueba de longitud de lista al eliminar elementos */
		for (int j = 0; j < longitud[i]/2; j++) lista_borrar_primero(lista);
		if (lista_largo(lista) != longitud[i]/2) correcto_largo = false;
		
		/* Vacío la lista */
		while (!lista_esta_vacia(lista)) lista_borrar_primero(lista);
		destruir_vector(vector,longitud[i]);
	}
	print_test("La lista guarda correctamente su longitud", correcto_largo == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_enlistar_alternadamente(void) {
	lista_t* lista = lista_crear();
	
	/* Prueba de enlistado alternado */
	
	/* Vector de prueba */
	size_t longitud_vector = 10;
	size_t vector[] =  {4, 5, 3, 6, 2, 7, 1, 8, 0, 9};
	
	/* Enlistado de elementos */
	for (size_t i = 0; i < longitud_vector; i++) {
		if (i%2 == 0) lista_insertar_primero(lista, &vector[i]);
		else lista_insertar_ultimo(lista, &vector[i]);
	}

	/* Desenlistado de elementos */
	bool desenlistado = true;
	for (size_t i = 0; i < longitud_vector; i++) {
		size_t* quitado = lista_borrar_primero(lista);
		if (*quitado != i) desenlistado = false;
	}
	
	print_test("Al enlistar alternadamente entre el principio y el final, los elementos se desenlistan en el orden correcto", desenlistado == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_volumen_primero(size_t cant_elementos) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int** elementos = crear_vector(cant_elementos);
	if (elementos == NULL) {
		print_test("No se pudo realizar la prueba de volumen debido a que no hay memoria dinamica para un vector de elementos", elementos != NULL);
		return;
	}
	
	/* Prueba de enlistado a través del primero */
	bool enlistado_primero = true;
	bool enlistado_ultimo = true;
	int i = 0;
	while (i < cant_elementos) {
		lista_insertar_primero(lista,elementos[i]);
		if (lista_ver_primero(lista) != elementos[i]) enlistado_primero = false;
		if (lista_ver_ultimo(lista) != elementos[0]) enlistado_ultimo = false;
		i++;
	}

	/* Desenlistado de elementos */
	bool desenlistado = true;
	while (i > 0) {
		i--;
		int* quitado = lista_borrar_primero(lista);
		if (quitado != elementos[i]) desenlistado = false;
	}

	/* Prueba de lista vacía al desenlistar todos los elementos */
	bool vacio = true;
	if(!lista_esta_vacia(lista)) vacio = false;
	
	print_test("Al enlistar desde el principio, el primer elemento de la lista siempre es el último elemento enlistado", enlistado_primero == true);
	print_test("Al enlistar desde el principio, el último elemento de la lista siempre es el primer elemento enlistado", enlistado_ultimo == true);
	print_test("Al enlistar desde el principio, se cumple el orden al desenlistar", desenlistado == true);
	print_test("Al enlistar desde el principio, los elementos se desenlistan hasta llegar al vacio", vacio == true);
	
	/* Destruyo vector y lista */
	destruir_vector(elementos,cant_elementos);
	lista_destruir(lista, NULL);
}

static void prueba_volumen_ultimo(size_t cant_elementos) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int** elementos = crear_vector(cant_elementos);
	if (elementos == NULL) {
		print_test("No se pudo realizar la prueba de volumen debido a que no hay memoria dinamica para un vector de elementos", elementos != NULL);
		return;
	}
	
	/* Prueba de enlistado a través del último */
	bool enlistado_primero = true;
	bool enlistado_ultimo = true;
	int i = 0;
	while (i < cant_elementos) {
		lista_insertar_ultimo(lista,elementos[i]);
		if (lista_ver_primero(lista) != elementos[0]) enlistado_primero = false;
		if (lista_ver_ultimo(lista) != elementos[i]) enlistado_ultimo = false;
		i++;
	}

	/* Desenlistado de elementos */
	bool desenlistado = true;
	while (i > 0) {
		int* quitado = lista_borrar_primero(lista);
		if (quitado != elementos[cant_elementos - i]) desenlistado = false;
		i--;
	}

	/* Prueba de lista vacía al desenlistar todos los elementos */
	bool vacio = true;
	if(!lista_esta_vacia(lista)) vacio = false;
	
	print_test("Al enlistar desde el final, el primer elemento de la lista siempre es el último elemento enlistado", enlistado_primero == true);
	print_test("Al enlistar desde el final, el último elemento de la lista siempre es el primer elemento enlistado", enlistado_ultimo == true);
	print_test("Al enlistar desde el final, se cumple el orden al desenlistar", desenlistado == true);
	print_test("Al enlistar desde el final, los elementos se desenlistan hasta llegar al vacio", vacio == true);
	
	/* Destruyo vector y lista */
	destruir_vector(elementos,cant_elementos);
	lista_destruir(lista, NULL);
}

static void prueba_enlistar_null(void) {
	/* Prueba de enlistar elemento NULL */
	lista_t* lista = lista_crear();
	bool enlistar_null = lista_insertar_primero(lista, NULL);
	if (lista_ver_primero(lista) != NULL) enlistar_null = false;

	print_test("El elemento NULL es enlistable", enlistar_null == true);

	/* Destruyo cola */
	lista_destruir(lista, NULL);
}

static void prueba_enlistar_vaciar_recien_creada(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int cant_elementos = 10000;
	int** elementos = crear_vector(cant_elementos);
	if (elementos == NULL) {
		print_test("No se pudo realizar la prueba de la cola vaciada debido a que no hay memoria dinamica para un vector de elementos", elementos != NULL);
		return;
	}
	
	/* Enlistado de elementos */
	int i = 0;
	while (i < cant_elementos) {
		lista_insertar_primero(lista,elementos[i]);
		i++;
	}

	/* Desenlistado de elementos */
	while (i > 0) {
		lista_borrar_primero(lista);
		i--;
	}
	
	/* Prueba de que lista desenlistada se comporta como una lista vacía (no se ve primero ni se puede desenlistar) */
	bool desenlistado = true;
	if (lista_ver_primero(lista) != NULL || lista_borrar_primero(lista) != NULL) {
		desenlistado = false;
	}
	print_test("Al ser desenlistada, la lista se comporta como recién creada", desenlistado == true);
	
	/* Destruyo vector y lista */
	destruir_vector(elementos,cant_elementos);
	lista_destruir(lista, NULL);
}

static void prueba_lista_recien_creada_desenlistar_ver_primero(void) {
	lista_t* lista = lista_crear();
	bool recien_creada = true;

	/* Prueba de ver primero vacío y de no poder desenlistar al recién crear la lista */
	if (lista_ver_primero(lista) != NULL || lista_borrar_primero(lista) != NULL) recien_creada = false;
	print_test("La lista al recién crearse no puede verse el primero ni desencolar", recien_creada == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_lista_recien_creada_vacia(void) {
	lista_t* lista = lista_crear();

	/* Prueba de lista vacía al crearse */
	print_test("La cola al recién crearse está vacía", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_sin_funcion(void) {
	lista_t* lista = lista_crear();

	/* Prueba de destruir lista vacía con función de destrucción */
	print_test("Se destruye una lista vacía sin utilizar una función de destruccion (revisar valgrind para ver el resultado de esta prueba)", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_con_funcion(void) {
	lista_t* lista = lista_crear();

	/* Prueba de destruir lista vacía con función de destrucción */
	print_test("Se destruye una lista vacía utilizando una función de destruccion (revisar valgrind para ver el resultado de esta prueba)", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, free);
}

static void prueba_enlistar_colas_destruir(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchas colas */
	int cant_colas = 100;
	
	/* Enlistado de colas */
	int i = 0;
	while (i < cant_colas) {
		lista_insertar_primero(lista,cola_crear());
		i++;
	}
	
	/* Destruyo lista utilizando la función correspondiente para las colas */
	lista_destruir(lista,destruir_cola);
	print_test("Se enlistan colas y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_colas);
}

static void prueba_enlistar_pilas_destruir(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchas pilas */
	int cant_pilas = 100;
	
	/* Enlistado de pilas */
	int i = 0;
	while (i < cant_pilas) {
		lista_insertar_primero(lista,pila_crear());
		i++;
	}
	
	/* Destruyo lista utilizando la función correspondiente para las pilas */
	lista_destruir(lista,destruir_pila);
	print_test("Se enlistan pilas y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_pilas);
}

static void prueba_enlistar_vectores_destruir(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos vectores */
	int cant_vecs = 100;
	size_t tam_vec = 2;
	
	/* Enlistado de vectores */
	int i = 0;
	while (i < cant_vecs) {
		lista_insertar_primero(lista,vector_crear(tam_vec));
		i++;
	}
	
	/* Destruyo lista utilizando la función correspondiente para los vectores */
	lista_destruir(lista,destruir_vector_TDA);
	print_test("Se enlistan vectores y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_vecs);
}

static void prueba_enlistar_punterosdinamicos_destruir(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos punteros */
	int cant_punts = 100;
	
	/* Enlistado de punteros */
	int i = 0;
	while (i < cant_punts) {
		void* puntero = malloc(sizeof(void*));
		if (puntero == NULL) {
			print_test("No se pudo probar el enlistado de punteros debido a que no hay memoria dinamica para un puntero", puntero != NULL);
			lista_destruir(lista,free);
			return;
		}
		lista_insertar_primero(lista,puntero);
		i++;
	}
	
	/* Destruyo lista utilizando la función correspondiente para los punteros */
	lista_destruir(lista,free);
	print_test("Se enlistan punteros y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_punts);
}

void pruebas_lista_estudiante() {
    /* Ejecuta todas las pruebas. */
    prueba_crear_destruir();
    prueba_largo_lista();
    prueba_enlistar_alternadamente();
    prueba_volumen_primero(10000);
    prueba_volumen_ultimo(10000);
    prueba_enlistar_null();
    prueba_enlistar_vaciar_recien_creada();
    prueba_lista_recien_creada_desenlistar_ver_primero();
    prueba_lista_recien_creada_vacia();
    prueba_destruir_lista_vacia_sin_funcion();
    prueba_destruir_lista_vacia_con_funcion();
    prueba_enlistar_colas_destruir();
    prueba_enlistar_pilas_destruir();
    prueba_enlistar_vectores_destruir();
    prueba_enlistar_punterosdinamicos_destruir();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
