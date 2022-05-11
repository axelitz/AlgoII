#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "lista.h"
#include "pila.h"

typedef struct abb_nodo {
	struct abb_nodo* izq;
	struct abb_nodo* der;
	char* clave;
	void* dato;
} abb_nodo_t;

struct abb {
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cant;
};

struct abb_iter {
	pila_t* pila;
};

/* *****************************************************************
 *             FUNCIONES AUXILIARES PARA NODO DE ABB
 * *****************************************************************/
 
char* strdup(const char* org) {
	char* cpy = malloc(sizeof(char) * (strlen(org) + 1));
	if (!cpy) return NULL;
	strcpy(cpy, org);
	return cpy;
}

abb_nodo_t* abb_nodo_crear(const char* clave, void* dato) {
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (nodo == NULL) return NULL;

	nodo->clave = strdup(clave);
	if (nodo->clave == NULL) {
		free(nodo);
		return NULL;
	}
	nodo->dato = dato;
	nodo->der = NULL;
	nodo->izq = NULL;

	return nodo;
}

void* abb_nodo_destruir(abb_nodo_t* nodo, abb_nodo_t* padre) {
	if (padre != NULL) {
		if (padre->der == nodo) padre->der = NULL;
		else padre->izq = NULL;
	}

	free(nodo->clave);
	void* dato = nodo->dato;
	free(nodo);
	return dato;
}

/* *****************************************************************
 *                 FUNCIONES AUXILIARES PARA ABB
 * *****************************************************************/

bool abb_insertar(abb_t* arbol, abb_nodo_t* padre, const char* clave, void* dato) {
	abb_nodo_t* nodo_nuevo = abb_nodo_crear(clave, dato);
	if (nodo_nuevo == NULL) return false;

	if (arbol->cmp(clave, padre->clave) > 0) padre->der = nodo_nuevo;
	else padre->izq = nodo_nuevo;

	arbol->cant++;
	return true;
}
 
void* eliminar_padre_de_un_hijo(abb_nodo_t* nodo, abb_nodo_t* padre, abb_comparar_clave_t cmp) {
	abb_nodo_t* nuevo_hijo = nodo->izq != NULL ? nodo->izq : nodo->der;
	void* dato = abb_nodo_destruir(nodo, padre);

	if (padre != NULL) {
		if (cmp(padre->clave, nuevo_hijo->clave) > 0) padre->izq = nuevo_hijo;
		else padre->der = nuevo_hijo;
	}
	return dato;
}

char* encontrar_siguiente_in_order(abb_nodo_t* nodo, const char* clave_a_comparar, abb_comparar_clave_t cmp) {
	if (nodo == NULL) return NULL;

	char* por_izquierda = encontrar_siguiente_in_order(nodo->izq, clave_a_comparar, cmp);
	if (por_izquierda != NULL) return por_izquierda;

	if (cmp(clave_a_comparar, nodo->clave) < 0) return strdup(nodo->clave);

	char* por_derecha = encontrar_siguiente_in_order(nodo->der, clave_a_comparar, cmp);
	return por_derecha;
}

void* eliminar_padre_de_dos_hijos(abb_nodo_t* nodo, abb_t* arbol) {
	char* clave_reemplazante = encontrar_siguiente_in_order(arbol->raiz, nodo->clave, arbol->cmp);
	if (clave_reemplazante == NULL) return NULL;
	void* dato_reemplazante = abb_borrar(arbol, clave_reemplazante);

	char* clave_vieja = nodo->clave;
	void* dato_viejo = nodo->dato;

	nodo->clave = clave_reemplazante;
	nodo->dato = dato_reemplazante;

	free(clave_vieja);
	return dato_viejo;
}

void* abb_borrar_segun_hijos(abb_nodo_t* nodo, abb_nodo_t* padre, abb_t* arbol, const char* clave) {
	if (nodo->izq == NULL && nodo->der == NULL) {
		arbol->cant--;
		if (nodo == arbol->raiz) arbol->raiz = NULL;
		return abb_nodo_destruir(nodo, padre);
	}
	if (nodo->izq == NULL || nodo->der == NULL) {
		arbol->cant--;
		if (nodo == arbol->raiz) arbol->raiz = nodo->izq != NULL ? nodo->izq : nodo->der;
		return eliminar_padre_de_un_hijo(nodo, padre, arbol->cmp);
	}
	return eliminar_padre_de_dos_hijos(nodo, arbol);
}

/* *****************************************************************
 *                     PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* arbol = malloc(sizeof(abb_t));
	if (arbol == NULL) return NULL;
	
	arbol->raiz = NULL;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	arbol->cant = 0;
	
	return arbol;
}

bool _abb_guardar(abb_nodo_t* nodo, abb_t* arbol, const char* clave, void *dato) {
	if (nodo == NULL) return false;

	int comparacion_claves = arbol->cmp(clave, nodo->clave);

	if (comparacion_claves == 0) {
		if (arbol->destruir_dato != NULL) arbol->destruir_dato(nodo->dato);
		nodo->dato = dato;
		return true;
	}

	abb_nodo_t* nodo_sig = comparacion_claves > 0 ? nodo->der : nodo->izq;
	return !_abb_guardar(nodo_sig, arbol, clave, dato) ? abb_insertar(arbol, nodo, clave, dato) : true;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato) {
	if (arbol->raiz == NULL) {
		arbol->raiz = abb_nodo_crear(clave, dato);
		if (arbol->raiz != NULL) arbol->cant++;
		return arbol->raiz != NULL;
	}

	return _abb_guardar(arbol->raiz, arbol, clave, dato);
}

void* _abb_borrar(abb_nodo_t* nodo, abb_nodo_t* padre, abb_t* arbol, const char* clave) {
	if (nodo == NULL) return NULL;

	int comparacion_claves = arbol->cmp(clave, nodo->clave);
	if (comparacion_claves == 0) return abb_borrar_segun_hijos(nodo, padre, arbol, clave);
	return comparacion_claves > 0 ? _abb_borrar(nodo->der, nodo, arbol, clave) : _abb_borrar(nodo->izq, nodo, arbol, clave);
}

void* abb_borrar(abb_t* arbol, const char* clave) {
	return _abb_borrar(arbol->raiz, NULL, arbol, clave);
}

void* _abb_obtener(abb_nodo_t* nodo, const abb_t* arbol, const char* clave) {
	if (nodo == NULL) return NULL;

	int comparacion_claves = arbol->cmp(clave, nodo->clave);
	if (comparacion_claves == 0) return nodo->dato;
	return comparacion_claves > 0 ? _abb_obtener(nodo->der, arbol, clave) : _abb_obtener(nodo->izq, arbol, clave);
}

void* abb_obtener(const abb_t* arbol, const char* clave) {
	return _abb_obtener(arbol->raiz, arbol, clave);
}

bool _abb_pertenece(abb_nodo_t* nodo, const abb_t* arbol, const char* clave) {
	if (nodo == NULL) return false;

	int comparacion_claves = arbol->cmp(clave, nodo->clave);
	if (comparacion_claves == 0) return true;
	return comparacion_claves > 0 ? _abb_pertenece(nodo->der, arbol, clave) : _abb_pertenece(nodo->izq, arbol, clave);
}

bool abb_pertenece(const abb_t* arbol, const char* clave) {
	return _abb_pertenece(arbol->raiz, arbol, clave);
}

size_t abb_cantidad(abb_t* arbol) {
	return arbol->cant;
}

void _abb_destruir(abb_nodo_t* nodo, abb_nodo_t* padre, abb_destruir_dato_t destruir_dato) {
	if (nodo == NULL) return;

	_abb_destruir(nodo->izq, nodo, destruir_dato);
	_abb_destruir(nodo->der, nodo, destruir_dato);

	void* dato = abb_nodo_destruir(nodo, padre);
	if (destruir_dato != NULL) destruir_dato(dato);
}

void abb_destruir(abb_t* arbol) {
	_abb_destruir(arbol->raiz, NULL, arbol->destruir_dato);
	free(arbol);
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

bool _abb_in_order(abb_nodo_t* nodo, bool visitar(const char*, void*, void*), void *extra) {
	if (nodo == NULL) return true;

	if (!_abb_in_order(nodo->izq, visitar, extra)) return false;
	if (!visitar(nodo->clave, nodo->dato, extra)) return false;
	return _abb_in_order(nodo->der, visitar, extra);
}

void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra) {
	_abb_in_order(arbol->raiz, visitar, extra);
}


/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/
 
abb_iter_t* abb_iter_in_crear(const abb_t* arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t*));
	if (iter == NULL) return NULL;
	
	iter->pila = pila_crear();
	if (iter->pila == NULL) {
		free(iter);
		return NULL;
	}

	abb_nodo_t* nodo = arbol->raiz;
	while (nodo != NULL) {
		if (!pila_apilar(iter->pila, nodo)) {
			abb_iter_in_destruir(iter);
			return NULL;
		}
		nodo = nodo->izq;
	}
	
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if (abb_iter_in_al_final(iter)) return false;
	
	abb_nodo_t* nodo_desapilado = pila_desapilar(iter->pila);
	abb_nodo_t* nodo_tope = pila_ver_tope(iter->pila);
	
	abb_nodo_t* nodo_apilar = nodo_desapilado->der;
	while (nodo_apilar != NULL) {
		if (!pila_apilar(iter->pila, nodo_apilar)) {
			// Si no logro apilar correctamente para cumplir el in-order, 
			// trato de dejar todo lo más parecido a como estaba antes.
			// (en principio dejo al siguiente del desapilado en el tope, 
			// y si puedo vuelvo a apilar al que originalmente quería desapiilar)
			abb_nodo_t* nodo_actual = pila_ver_tope(iter->pila);
			while (nodo_actual != nodo_tope) nodo_actual = pila_desapilar(iter->pila);
			pila_apilar(iter->pila, nodo_desapilado);
			return false;
		}
		nodo_apilar = nodo_apilar->izq;
	}
	return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t *iter) {
	abb_nodo_t* nodo = pila_ver_tope(iter->pila);
	return nodo != NULL ? nodo->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t *iter) {
	pila_destruir(iter->pila);
	free(iter);
}
