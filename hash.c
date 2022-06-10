#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "lista.h"

#define TAM_INICIAL 101
#define ALFA_MINIMO 0.25f
#define ALFA_MAXIMO 2
#define FACTOR_CREC 2
#define FACTOR_DEC 0.5f
#define BASE 256

/* Estructura para almacenar clave, dato */
typedef struct hash_campo {
	char* clave;
	void* dato;
} hash_campo_t;

/* Estructura del hash: arreglo de punteros a lista. */
struct hash {
	lista_t** tabla;
	size_t cantidad;
	size_t largo;
	hash_destruir_dato_t destruir_dato;
};

/* Estructura del iterador. */
struct hash_iter {
	const hash_t* hash;
	size_t indice;
	lista_iter_t* iter_actual;
};

/* *****************************************************************
 *             	FUNCIÓN DE HASHING
 * *****************************************************************/
 
unsigned long hashing(const char *s, unsigned long m) {
	unsigned long h;
	unsigned const char *us;
	us = (unsigned const char *) s;

	h = 0;
	while (*us != '\0') {
		h = (h * BASE + *us) % m;
		us++;
	} 

	return h;
}

/* *****************************************************************
 *             FUNCIONES AUXILIARES PARA CAMPO DE HASH
 * *****************************************************************/

char* strdup(const char* org) {
	char* cpy = malloc(sizeof(char) * (strlen(org) + 1));
	if (!cpy) return NULL;
	strcpy(cpy, org);
	return cpy;
}

hash_campo_t* hash_campo_crear(const char* clave, void* dato) {
	hash_campo_t* campo = malloc(sizeof(hash_campo_t));
	if (campo == NULL) return NULL;

	campo->clave = strdup(clave);
	campo->dato = dato;

	return campo;
}

void* hash_campo_destruir(hash_campo_t* campo) {
	free(campo->clave);
	void* dato = campo->dato;
	free(campo);
	return dato;
}

void destruir_campo_hash(void* campo) {
	hash_campo_destruir(campo);
}

/* *****************************************************************
 *                 FUNCIONES AUXILIARES PARA HASH
 * *****************************************************************/

lista_t** tabla_crear(size_t n) {
	lista_t** tabla = malloc(n*sizeof(lista_t*));
	if (tabla == NULL) return NULL;
	
	for (int i = 0; i < n; i++) {
		tabla[i] = lista_crear();
		if (tabla[i] == NULL) {
			while (i-- >= 0) lista_destruir(tabla[i], NULL);
			free(tabla);
			return NULL;
		}
	}
	return tabla;
}

void tabla_destruir(lista_t** tabla, size_t n, hash_destruir_dato_t destruir_dato, void (*destruir_campo)(void*)) {
	for (int i = 0; i < n; i++) {
		if (destruir_dato != NULL) {
			lista_iter_t* iter = lista_iter_crear(tabla[i]);
			while(!lista_iter_al_final(iter)) {
				hash_campo_t* campo = lista_iter_borrar(iter);
				void* dato = hash_campo_destruir(campo);
				destruir_dato(dato);
			}
			lista_iter_destruir(iter);
		}
		lista_destruir(tabla[i], destruir_campo);
	}
	free(tabla);
}

bool llenar_tabla_redmiensionada_a_partir_de_hash(lista_t** tabla_nueva, size_t largo_nuevo, hash_t* hash) {
	for (int i = 0; i < hash->largo; i++) {
		lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
		if (iter == NULL) {
			tabla_destruir(tabla_nueva, largo_nuevo, NULL, NULL);
			return false;
		}
	  	while (!lista_iter_al_final(iter)) {
			hash_campo_t* campo = lista_iter_ver_actual(iter);
			unsigned long indice = hashing(campo->clave, largo_nuevo);
			if (!lista_insertar_ultimo(tabla_nueva[indice], campo)) {
				tabla_destruir(tabla_nueva, largo_nuevo, NULL, NULL);
					lista_iter_destruir(iter);
				return false;
			}
			lista_iter_avanzar(iter);
	   	}
		lista_iter_destruir(iter);
	}
	return true;
}

void reemplazar_tablas_en_hash(hash_t* hash, lista_t** tabla_nueva, size_t largo_nuevo) {
	lista_t** aux = hash->tabla;
	hash->tabla = tabla_nueva;
	tabla_destruir(aux, hash->largo, NULL, NULL);
	hash->largo = largo_nuevo;	
}

bool hash_redimensionar(hash_t* hash, float factor_crecimiento) {
	size_t valor_ajuste = (size_t)(factor_crecimiento*(float)hash->largo);
	if (valor_ajuste >= TAM_INICIAL) {
		lista_t** tabla_nueva = tabla_crear(valor_ajuste);
		if (tabla_nueva == NULL) return false;

		if(!llenar_tabla_redmiensionada_a_partir_de_hash(tabla_nueva, valor_ajuste, hash)) return false;
		reemplazar_tablas_en_hash(hash, tabla_nueva, valor_ajuste);
	}
	return true;
}

bool redimensionamiento(hash_t* hash) {
	float alfa = (float)(hash->cantidad/hash->largo);
	if ((alfa >= ALFA_MAXIMO && !hash_redimensionar(hash, FACTOR_CREC)) || (alfa <= ALFA_MINIMO && !hash_redimensionar(hash, FACTOR_DEC))) return false;
	return true;
}

lista_iter_t* encontrar_en_hash_segun_clave(const hash_t* hash, const char* clave) {
	unsigned long indice = hashing(clave, hash->largo);
	lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
	if (iter == NULL) return NULL;

	hash_campo_t* campo = lista_iter_ver_actual(iter);
	while (!lista_iter_al_final(iter) && strcmp(campo->clave,clave) != 0) {
		lista_iter_avanzar(iter);
		campo = lista_iter_ver_actual(iter);
	}
	return iter;
}

/* *****************************************************************
 *            FUNCIONES AUXILIARES PARA ITERADOR DE HASH
 * *****************************************************************/

void buscar_siguiente_lista_valida(hash_iter_t* iter) {
	while (iter->indice < iter->hash->largo && lista_esta_vacia(iter->hash->tabla[iter->indice])) iter->indice++;
	if (iter->indice < iter->hash->largo) iter->iter_actual = lista_iter_crear(iter->hash->tabla[iter->indice]);
}

/* *****************************************************************
 *                     PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if (hash == NULL) return NULL;

	hash->largo = TAM_INICIAL;
	hash->tabla = tabla_crear(hash->largo);
	if (hash->tabla == NULL) {
		free(hash);
		return NULL;
	}
	hash->cantidad = 0;
	hash->destruir_dato = destruir_dato;

	return hash;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
	hash_campo_t* campo = hash_campo_crear(clave, dato);
	if (campo == NULL) return false;
	
	if (!redimensionamiento(hash)) return false;

	if (hash_pertenece(hash, clave)) {
		void* dato_borrado = hash_borrar(hash, clave);
		if (hash->destruir_dato != NULL) hash->destruir_dato(dato_borrado);
	}

	hash->cantidad++;
	unsigned long indice = hashing(campo->clave, hash->largo);
	return lista_insertar_ultimo(hash->tabla[indice], campo);
}

void* hash_borrar(hash_t* hash, const char* clave) {
	if (!redimensionamiento(hash)) return NULL;

	lista_iter_t* iter = encontrar_en_hash_segun_clave(hash, clave);
	if (iter == NULL) return NULL;
	if (lista_iter_al_final(iter)) {
		lista_iter_destruir(iter);
		return NULL;
	}

	hash->cantidad--;
	hash_campo_t* campo = lista_iter_borrar(iter);
	lista_iter_destruir(iter);
	void* dato = hash_campo_destruir(campo);
	return dato;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
	lista_iter_t* iter = encontrar_en_hash_segun_clave(hash, clave);
	if (iter == NULL) return NULL;
	if (lista_iter_al_final(iter)) {
		lista_iter_destruir(iter);
		return NULL;
	}

	hash_campo_t* campo = lista_iter_ver_actual(iter);
	lista_iter_destruir(iter);
	void* dato = campo->dato;
	return dato;
}

bool hash_pertenece(const hash_t* hash, const char* clave) {
	lista_iter_t* iter = encontrar_en_hash_segun_clave(hash, clave);
	if (iter == NULL) return false;
	bool pertenece = !lista_iter_al_final(iter);
	lista_iter_destruir(iter);
	return pertenece;
}

size_t hash_cantidad(const hash_t* hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t* hash) {
	tabla_destruir(hash->tabla, hash->largo, hash->destruir_dato, destruir_campo_hash);
	free(hash);
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR DEL HASH
 * *****************************************************************/

hash_iter_t* hash_iter_crear(const hash_t* hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (iter == NULL) return NULL;

	iter->hash = hash; 
	iter->indice = 0;
	iter->iter_actual = NULL;
	buscar_siguiente_lista_valida(iter);

	return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter) {
	if (iter->iter_actual == NULL) return false;
	if (lista_iter_avanzar(iter->iter_actual) && !lista_iter_al_final(iter->iter_actual)) return true;

	lista_iter_destruir(iter->iter_actual);
	iter->iter_actual = NULL;
	iter->indice++;
	buscar_siguiente_lista_valida(iter);
	return iter->indice < iter->hash->largo;
}

const char* hash_iter_ver_actual(const hash_iter_t* iter) {
	return hash_iter_al_final(iter) ? NULL : ((hash_campo_t *)lista_iter_ver_actual(iter->iter_actual))->clave;
}

bool hash_iter_al_final(const hash_iter_t* iter) {
	return iter->indice == iter->hash->largo;
}

void hash_iter_destruir(hash_iter_t* iter) {
	if (iter->iter_actual != NULL) lista_iter_destruir(iter->iter_actual);
	free(iter);
}
