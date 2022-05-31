#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

#define TAM_INICIAL 10
#define FACTOR_CREC 2
#define FACTOR_DEC 0.5f

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

struct vector {
	void** datos;
	size_t cantidad;
	size_t capacidad;
	vector_destruir_dato_t destruir_dato;
};

/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/

vector_t* vector_crear(vector_destruir_dato_t destruir_dato) {
	vector_t* vector = malloc(sizeof(vector_t));
	if (vector == NULL) return NULL;

	vector->capacidad = TAM_INICIAL;

	vector->datos = malloc(vector->capacidad * sizeof(void*));
	if (vector->datos == NULL) {
		free(vector);
		return NULL;
    	}
    	
    	vector->cantidad = 0;
	vector->destruir_dato = destruir_dato;
	return vector;
}

bool vector_redimensionar(vector_t* vector, float factor_crecimiento) {
	size_t valor_ajuste = (size_t)(factor_crecimiento*(float)hash->largo);

	if (valor_ajuste >= TAM_INICIAL) {
		void** datos_nuevo =  realloc(vector->datos,valor_ajuste*sizeof(void*));
		if (datos_nuevo == NULL) return false;

		vector->datos = datos_nuevo;
		vector->capacidad = valor_ajuste;
	}
	return true;
}

bool vector_obtener(vector_t* vector, size_t pos, void* dato) {
	if (pos >= vector->capacidad) return false;

	dato = vector->datos[pos];
	return true;
}

bool vector_guardar(vector_t* vector, void* dato) {
	if (vector->cantidad == vector->capacidad && !vector_redimensionar(vector, FACTOR_CREC)) return false;

	vector->datos[vector->cantidad] = dato;
	return true;
}

size_t vector_cantidad(vector_t* vector) {
	return vector->cantidad;
}

void vector_destruir(vector_t* vector) {
	if (vector->destruir_dato != NULL) for (size_t i = 0; i < vector->cant; i++) destruir_dato(vector->datos[i]);
	free(vector->datos);
	free(vector);
}
