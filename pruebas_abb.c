#include "abb.h"
#include "testing.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.}

#define NUMERO_PRIMO_GRANDE 5003

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio() {
	printf("\nINICIO DE PRUEBAS ABB VACIO\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	print_test("Prueba ABB crear ABB vacio", arbol);
	print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
	print_test("Prueba ABB obtener clave A, es NULL, no existe", !abb_obtener(arbol, "A"));
	print_test("Prueba ABB pertenece clave A, es false, no existe", !abb_pertenece(arbol, "A"));
	print_test("Prueba ABB borrar clave A, es NULL, no existe", !abb_borrar(arbol, "A"));

	abb_destruir(arbol);
}

static void prueba_iterar_abb_vacio() {
	printf("\nINICIO DE PRUEBAS ITERAR ABB VACIO\n");
	abb_t* arbol = abb_crear(strcmp, NULL);
	abb_iter_t* iter = abb_iter_in_crear(arbol);

	print_test("Prueba ABB iter crear iterador ABB vacio", iter);
	print_test("Prueba ABB iter esta al final", abb_iter_in_al_final(iter));
	print_test("Prueba ABB iter avanzar es false", !abb_iter_in_avanzar(iter));
	print_test("Prueba ABB iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
}

static void prueba_abb_insertar() {
	printf("\nINICIO DE PRUEBAS INSERTAR EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "vaca", *valor3 = "mu";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba ABB insertar clave1", abb_guardar(arbol, clave1, valor1));
	print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Prueba ABB obtener clave1 es valor1", abb_obtener(arbol, clave1) == valor1);
	print_test("Prueba ABB pertenece clave1, es true", abb_pertenece(arbol, clave1));
	print_test("Prueba ABB borrar clave1, es valor1", abb_borrar(arbol, clave1) == valor1);
	print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	/* Inserta otros 2 valores y no los borra (se destruyen con el hash) */
	print_test("Prueba ABB insertar clave2", abb_guardar(arbol, clave2, valor2));
	print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Prueba ABB obtener clave2 es valor2", abb_obtener(arbol, clave2) == valor2);
	print_test("Prueba ABB pertenece clave2, es true", abb_pertenece(arbol, clave2));

	print_test("Prueba ABB insertar clave3", abb_guardar(arbol, clave3, valor3));
	print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(arbol) == 2);
	print_test("Prueba ABB obtener clave3 es valor3", abb_obtener(arbol, clave3) == valor3);
	print_test("Prueba ABB pertenece clave3, es true", abb_pertenece(arbol, clave3));

	abb_destruir(arbol);
}

static void prueba_abb_reemplazar() {
	printf("\nINICIO DE PRUEBAS REEMPLAZAR EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
	char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

	/* Inserta 2 valores y luego los reemplaza */
	print_test("Prueba ABB insertar clave1", abb_guardar(arbol, clave1, valor1a));
	print_test("Prueba ABB obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
	print_test("Prueba ABB insertar clave2", abb_guardar(arbol, clave2, valor2a));
	print_test("Prueba ABB obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
	print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	print_test("Prueba ABB insertar clave1 con otro valor", abb_guardar(arbol, clave1, valor1b));
	print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
	print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
	print_test("Prueba ABB insertar clave2 con otro valor", abb_guardar(arbol, clave2, valor2b));
	print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
	print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
	print_test("Prueba hash la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	abb_destruir(arbol);
}

static void prueba_abb_reemplazar_con_destruir() {
	printf("\nINICIO DE PRUEBAS REEMPLAZAR CON DESTRUIR EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, free);

	char *clave1 = "perro", *valor1a, *valor1b;
	char *clave2 = "gato", *valor2a, *valor2b;

	/* Pide memoria para 4 valores */
	valor1a = malloc(10 * sizeof(char));
	valor1b = malloc(10 * sizeof(char));
	valor2a = malloc(10 * sizeof(char));
	valor2b = malloc(10 * sizeof(char));

	/* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
	print_test("Prueba ABB insertar clave1", abb_guardar(arbol, clave1, valor1a));
	print_test("Prueba ABB obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
	print_test("Prueba ABB insertar clave2", abb_guardar(arbol, clave2, valor2a));
	print_test("Prueba ABB obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
	print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	print_test("Prueba ABB insertar clave1 con otro valor", abb_guardar(arbol, clave1, valor1b));
	print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
	print_test("Prueba ABB insertar clave2 con otro valor", abb_guardar(arbol, clave2, valor2b));
	print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
	print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	/* Se destruye el ABB (se debe liberar lo que quedó dentro) */
	abb_destruir(arbol);
}

static void prueba_abb_borrar() {
	printf("\nINICIO DE PRUEBAS BORRAR EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "vaca", *valor3 = "mu";

	/* Inserta 3 valores y luego los borra */
	print_test("Prueba ABB insertar clave1", abb_guardar(arbol, clave1, valor1));
	print_test("Prueba ABB insertar clave2", abb_guardar(arbol, clave2, valor2));
	print_test("Prueba ABB insertar clave3", abb_guardar(arbol, clave3, valor3));

	/* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
	print_test("Prueba ABB pertenece clave3, es verdadero", abb_pertenece(arbol, clave3));
	print_test("Prueba ABB borrar clave3, es valor3", abb_borrar(arbol, clave3) == valor3);
	print_test("Prueba ABB borrar clave3, es NULL", !abb_borrar(arbol, clave3));
	print_test("Prueba ABB pertenece clave3, es falso", !abb_pertenece(arbol, clave3));
	print_test("Prueba ABB obtener clave3, es NULL", !abb_obtener(arbol, clave3));
	print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	print_test("Prueba ABB pertenece clave1, es verdadero", abb_pertenece(arbol, clave1));
	print_test("Prueba ABB borrar clave1, es valor1", abb_borrar(arbol, clave1) == valor1);
	print_test("Prueba ABB borrar clave1, es NULL", !abb_borrar(arbol, clave1));
	print_test("Prueba ABB pertenece clave1, es falso", !abb_pertenece(arbol, clave1));
	print_test("Prueba ABB obtener clave1, es NULL", !abb_obtener(arbol, clave1));
	print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(arbol) == 1);

	print_test("Prueba ABB pertenece clave2, es verdadero", abb_pertenece(arbol, clave2));
	print_test("Prueba ABB borrar clave2, es valor2", abb_borrar(arbol, clave2) == valor2);
	print_test("Prueba ABB borrar clave2, es NULL", !abb_borrar(arbol, clave2));
	print_test("Prueba ABB pertenece clave2, es falso", !abb_pertenece(arbol, clave2));
	print_test("Prueba ABB obtener clave2, es NULL", !abb_obtener(arbol, clave2));
	print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	abb_destruir(arbol);
}

static void prueba_abb_clave_vacia() {
	printf("\nINICIO DE PRUEBAS CLAVE VACIA EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	char *clave = "", *valor = "";

	print_test("Prueba ABB insertar clave vacia", abb_guardar(arbol, clave, valor));
	print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Prueba ABB obtener clave vacia es valor", abb_obtener(arbol, clave) == valor);
	print_test("Prueba ABB pertenece clave vacia, es true", abb_pertenece(arbol, clave));
	print_test("Prueba ABB borrar clave vacia, es valor", abb_borrar(arbol, clave) == valor);
	print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	abb_destruir(arbol);
}

static void prueba_abb_valor_null() {
	printf("\nINICIO DE PRUEBAS VALOR NULL EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	char *clave = "", *valor = NULL;

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba ABB insertar clave vacia valor NULL", abb_guardar(arbol, clave, valor));
	print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Prueba ABB obtener clave vacia es valor NULL", abb_obtener(arbol, clave) == valor);
	print_test("Prueba ABB pertenece clave vacia, es true", abb_pertenece(arbol, clave));
	print_test("Prueba ABB borrar clave vacia, es valor NULL", abb_borrar(arbol, clave) == valor);
	print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	abb_destruir(arbol);
}

bool es_primo (size_t n) {
	for (int i = 2; i <= n/2; i++) if(n % i == 0) return false;
	return true;
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

static void prueba_abb_volumen(size_t largo, bool debug) {
	printf("\nINICIO DE PRUEBAS VOLUMEN EN ABB\n");
	if (!es_primo(largo)) {
		print_test("El largo del vector no es primo y por ende no se puede realizar esta prueba", es_primo(largo) == true);
		return;
	}
	abb_t* arbol = abb_crear(strcmp, NULL);

	const size_t largo_clave = 10;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);

	unsigned* valores[largo];

	/* Inserta 'largo' parejas en el ABB */
	size_t aleatorio[largo];
	for (size_t i = 0; i < largo; i++) aleatorio[i] = i+1;
	permutacion_aleatoria (aleatorio, largo);
	bool ok = true;
	for (unsigned i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		sprintf(claves[i], "%08ld", aleatorio[i]);
		*valores[i] = i+1;
		ok = abb_guardar(arbol, claves[i], valores[i]);
		if (!ok) break;
	}

	if (debug) print_test("Prueba ABB almacenar muchos elementos", ok);
	if (debug) print_test("Prueba ABB la cantidad de elementos es correcta", abb_cantidad(arbol) == largo);

	/* Verifica que devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = abb_pertenece(arbol, claves[i]);
		if (!ok) break;
		ok = abb_obtener(arbol, claves[i]) == valores[i];
		if (!ok) break;
	}

	if (debug) print_test("Prueba ABB pertenece y obtener muchos elementos", ok);
	if (debug) print_test("Prueba ABB la cantidad de elementos es correcta", abb_cantidad(arbol) == largo);

	/* Verifica que borre y devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = abb_borrar(arbol, claves[i]) == valores[i];
		if (!ok) break;
	}

	if (debug) print_test("Prueba ABB borrar muchos elementos", ok);
	if (debug) print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	/* Destruye el ABB y crea uno nuevo que sí libera */
	abb_destruir(arbol);
	arbol = abb_crear(strcmp, free);

	/* Inserta 'largo' parejas en el ABB */
	ok = true;
	for (size_t i = 0; i < largo; i++) {
		ok = abb_guardar(arbol, claves[i], valores[i]);
		if (!ok) break;
	}

	free(claves);

	/* Destruye el ABB - debería liberar los enteros */
	abb_destruir(arbol);
}
static ssize_t buscar(const char* clave, char* claves[], size_t largo) {
	for (size_t i = 0; i < largo; i++) {
		if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
	}
	return -1;
}

static void prueba_abb_iterar() {
	printf("\nINICIO DE PRUEBAS ITERACION EN ABB\n");
	abb_t* arbol = abb_crear(strcmp, NULL);

	char *claves[] = {"perro", "gato", "vaca"};
	char *valores[] = {"guau", "miau", "mu"};

	/* Inserta 3 valores */
	print_test("Prueba ABB insertar clave1", abb_guardar(arbol, claves[0], valores[0]));
	print_test("Prueba ABB insertar clave2", abb_guardar(arbol, claves[1], valores[1]));
	print_test("Prueba ABB insertar clave3", abb_guardar(arbol, claves[2], valores[2]));

	// Prueba de iteración sobre las claves almacenadas.
	abb_iter_t* iter = abb_iter_in_crear(arbol);
	const char *clave;
	ssize_t indice;

	print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

	/* Primer valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba ABB iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba ABB iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	print_test("Prueba ABB iterador avanzar es true", abb_iter_in_avanzar(iter));
	print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

	/* Segundo valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba ABB iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba ABB iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	print_test("Prueba ABB iterador avanzar es true", abb_iter_in_avanzar(iter));
	print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

	/* Tercer valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba ABB iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba ABB iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	abb_iter_in_avanzar(iter);
	print_test("Prueba ABB iterador esta al final, es true", abb_iter_in_al_final(iter));

	/* Vuelve a tratar de avanzar, por las dudas */
	print_test("Prueba ABB iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
	print_test("Prueba ABB iterador avanzar es false", !abb_iter_in_avanzar(iter));
	print_test("Prueba ABB iterador esta al final, es true", abb_iter_in_al_final(iter));

	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
}

static void prueba_abb_iterar_volumen(size_t largo) {
	printf("\nINICIO DE PRUEBAS VOLUMEN DE ITERACION EN ABB\n");
	if (!es_primo(largo)) {
		print_test("El largo del vector no es primo y por ende no se puede realizar esta prueba", es_primo(largo) == true);
		return;
	}
	abb_t* arbol = abb_crear(strcmp, NULL);

	const size_t largo_clave = 10;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);

	size_t valores[largo];

	/* Inserta 'largo' parejas en el hash */
	size_t aleatorio[largo];
	for (size_t i = 0; i < largo; i++) aleatorio[i] = i+1;
	permutacion_aleatoria (aleatorio, largo);
	bool ok = true;
	for (unsigned i = 0; i < largo; i++) {
		sprintf(claves[i], "%08ld", aleatorio[i]);
		valores[i] = i+1;
		ok = abb_guardar(arbol, claves[i], &valores[i]);
		if (!ok) break;
	}

	// Prueba de iteración sobre las claves almacenadas.
	abb_iter_t* iter = abb_iter_in_crear(arbol);
	print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

	ok = true;
	unsigned i;
	const char *clave;
	size_t *valor;

	for (i = 0; i < largo; i++) {
		if (abb_iter_in_al_final(iter)) {
			ok = false;
			break;
		}
		clave = abb_iter_in_ver_actual(iter);
		if (clave == NULL) {
			ok = false;
			break;
		}
		valor = abb_obtener(arbol, clave);
		if (valor == NULL) {
			ok = false;
			break;
		}
		*valor = largo;
		abb_iter_in_avanzar(iter);
	}
	print_test("Prueba ABB iteración en volumen", ok);
	print_test("Prueba ABB iteración en volumen, recorrio todo el largo", i == largo);
	print_test("Prueba ABB iterador esta al final, es true", abb_iter_in_al_final(iter));

	ok = true;
	for (i = 0; i < largo; i++) {
		if (valores[i] != largo) {
			ok = false;
			break;
		}
	}
	print_test("Prueba ABB iteración en volumen, se cambiaron todo los elementos", ok);

	free(claves);
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_abb_estudiante() {
	/* Ejecuta todas las pruebas unitarias. */
	prueba_crear_abb_vacio();
	prueba_iterar_abb_vacio();
	prueba_abb_insertar();
	prueba_abb_reemplazar();
	prueba_abb_reemplazar_con_destruir();
	prueba_abb_borrar();
	prueba_abb_clave_vacia();
	prueba_abb_valor_null();
	prueba_abb_volumen(NUMERO_PRIMO_GRANDE, true);
	prueba_abb_iterar();
	prueba_abb_iterar_volumen(NUMERO_PRIMO_GRANDE);
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
	pruebas_abb_estudiante();
	return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
