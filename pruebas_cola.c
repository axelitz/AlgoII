#include "cola.h"
#include "pila.h"
#include "vector.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

int** crear_vector(size_t cant) {
	/* Creo vector de tamaño cant */
	int** vector = malloc(cant*sizeof(int*));
	if (vector == NULL) {
		return NULL;
	}
	
	for(int i = 0; i < cant; i++) {
		vector[i] = malloc(sizeof(int*));
		if (vector[i] != NULL) *vector[i] = i;
	}
	return vector;
}

void destruir_vector(int** vector, size_t cant) {
	/* Destruyo el vector creado */
	for(int i = 0; i < cant; i++) {
		free(vector[i]);
	}
	free(vector);
}

void destruir_pila(void* pila) {
	pila_destruir(pila);
}

void destruir_vector_TDA(void* vector) {
	vector_destruir(vector);
}

// 1
static void prueba_crear_destruir(void) {
	/* Prueba de cola creada correctamente */
	cola_t* cola = cola_crear();
	print_test("La cola se pudo crear", cola != NULL);
	
	/* Destruyo cola */
	cola_destruir(cola, NULL);

}

// 2
static void prueba_FIFO(void) {
	/* Pŕueba más detallada del FIFO en la cola (analizando un puntero en particular) */
	cola_t* cola = cola_crear();
	
	/* Puntero a observar en la prueba */
	void* puntero = malloc(sizeof(void*));
	if (puntero == NULL) {
		print_test("No se pudo probar el FIFO debido a que no hay memoria dinamica para un puntero", puntero != NULL);
		return;
	}
	cola_encolar(cola, puntero);
	
	/* Encolo un vector */
	int cant_elementos = 5;
	int** elementos = crear_vector(cant_elementos);
	
	int i = 0;
	while (i < cant_elementos) {
		cola_encolar(cola,elementos[i]);
		i++;
	}
	
	/* Prueba del FIFO luego del encolado */
	print_test("La cola se encola y desencola correctamente, obteniendo el tope deseado", cola_ver_primero(cola) == puntero);
	
	/* Destruyo vector, puntero y cola */
	destruir_vector(elementos,cant_elementos);
	free(puntero);
	cola_destruir(cola, NULL);
}

// 3
static void prueba_volumen_encolar_desencolar(size_t cant_elementos) {
	cola_t* cola = cola_crear();
	
	/* Encolado con muchos elementos */
	int** elementos = crear_vector(cant_elementos);
	
	/* Encolado de elementos + Prueba de que el primero siempre corresponde al primer elemento agregado */
	bool encolado = true;
	int i = 0;
	while (i < cant_elementos) {
		cola_encolar(cola,elementos[i]);
		if (cola_ver_primero(cola) != elementos[0]) encolado = false;
		i++;
	}
	print_test("El primer elemento de la cola siempre es el primer elemento encolado", encolado == true);

	/* Desencolado de elementos + Prueba de que se respeta el FIFO elemento a elemento desencolado */
	bool desencolado = true;
	i = 0;
	while (i < cant_elementos) {
		int* quitado = cola_desencolar(cola);
		if (quitado != elementos[i]) {
			desencolado = false;
		}
		i++;
	}
	print_test("Se cumple el FIFO al desencolar", desencolado == true);

	/* Prueba de cola vacía al desencolar todos los elementos */
	bool vacio = true;
	if(!cola_esta_vacia(cola)) {
		vacio = false;
	}
	print_test("Los elementos se desencolan hasta llegar al vacio", vacio == true);
	
	/* Destruyo vector y cola */
	destruir_vector(elementos,cant_elementos);
	cola_destruir(cola, NULL);
}

// 4
static void prueba_encolar_null(void) {
	/* Prueba de encolar elemento NULL */
	cola_t* cola = cola_crear();
	bool encolar_null = cola_encolar(cola, NULL);
	if (cola_ver_primero(cola) != NULL) {
		encolar_null = false;
	}
	print_test("El elemento NULL es encolable", encolar_null == true);

	/* Destruyo cola */
	cola_destruir(cola, NULL);
}

// 5 + 8
static void prueba_encolar_vaciar_recien_creada(void) {
	cola_t* cola = cola_crear();
	
	/* Encolado con muchos elementos */
	int cant_elementos = 10000;
	int** elementos = crear_vector(cant_elementos);
	
	/* Encolado de elementos */
	int i = 0;
	while (i < cant_elementos) {
		cola_encolar(cola,elementos[i]);
		i++;
	}

	/* Desencolado de elementos */
	i = 0;
	while (i < cant_elementos) {
		cola_desencolar(cola);
		i++;
	}
	
	/* Prueba de que cola desencolada se comporta como una cola vacía (no se ve primero ni se puede desencolar) */
	bool desencolado = true;
	if (cola_ver_primero(cola) != NULL || cola_desencolar(cola) != NULL) {
		desencolado = false;
	}
	print_test("Al ser desencolada, la cola se comporta como recién creada", desencolado == true);
	
	/* Destruyo vector y cola */
	destruir_vector(elementos,cant_elementos);
	cola_destruir(cola, NULL);
}

// 6
static void prueba_cola_recien_creada_desencolar_ver_primero(void) {
	cola_t* cola = cola_crear();
	bool recien_creada = true;

	/* Prueba de ver primero vacío y de no poder desencolar al recién crear la cola */
	if (cola_ver_primero(cola) != NULL || cola_desencolar(cola) != NULL) {
		recien_creada = false;
	}
	print_test("La cola al recién crearse no puede verse el primero ni desencolar", recien_creada == true);
	
	/* Destruyo cola */
	cola_destruir(cola, NULL);
}

// 7
static void prueba_cola_recien_creada_vacia(void) {
	cola_t* cola = cola_crear();

	/* Prueba de cola vacía al crearse */
	print_test("La cola al recién crearse está vacía", cola_esta_vacia(cola) == true);
	
	/* Destruyo cola */
	cola_destruir(cola, NULL);
}

static void prueba_encolar_pilas_destruir(void) {
	cola_t* cola = cola_crear();
	
	/* Encolado con muchas pilas */
	int cant_pilas = 100;
	
	/* Encolado de pilas */
	int i = 0;
	while (i < cant_pilas) {
		cola_encolar(cola,pila_crear());
		i++;
	}
	
	/* Destruyo cola utilizando la función correspondiente para las pilas */
	cola_destruir(cola,destruir_pila);
	print_test("Se encolan pilas y luego la cola es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_pilas);
}

static void prueba_encolar_vectores_destruir(void) {
	cola_t* cola = cola_crear();
	
	/* Encolado con muchos vectores */
	int cant_vecs = 100;
	size_t tam_vec = 2;
	
	/* Encolado de vectores */
	int i = 0;
	while (i < cant_vecs) {
		cola_encolar(cola,vector_crear(tam_vec));
		i++;
	}
	
	/* Destruyo cola utilizando la función correspondiente para los vectores */
	cola_destruir(cola,destruir_vector_TDA);
	print_test("Se encolan vectores y luego la cola es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_vecs);
}

static void prueba_encolar_punterosdinamicos_destruir(void) {
	cola_t* cola = cola_crear();
	
	/* Encolado con muchos punteros */
	int cant_punts = 100;
	
	/* Encolado de punteros */
	int i = 0;
	while (i < cant_punts) {
		void* puntero = malloc(sizeof(void*));
		if (puntero == NULL) {
			print_test("No se pudo probar el FIFO debido a que no hay memoria dinamica para un puntero", puntero != NULL);
			cola_destruir(cola,free);
			return;
		}
		cola_encolar(cola,puntero);
		i++;
	}
	
	/* Destruyo cola utilizando la función correspondiente para los punteros */
	cola_destruir(cola,free);
	print_test("Se encolan punteros y luego la cola es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_punts);
}

void pruebas_cola_estudiante() {
    /* Ejecuta todas las pruebas. */
    prueba_crear_destruir(); // 1
    prueba_FIFO(); // 2
    prueba_volumen_encolar_desencolar(10000); // 3
    prueba_encolar_null(); // 4
    prueba_encolar_vaciar_recien_creada(); // 5 + 8
    prueba_cola_recien_creada_desencolar_ver_primero(); // 6
    prueba_cola_recien_creada_vacia(); // 7
    prueba_encolar_pilas_destruir();
    prueba_encolar_vectores_destruir();
    prueba_encolar_punterosdinamicos_destruir();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
