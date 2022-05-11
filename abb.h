#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>
#include "lista.h"

/* Tipos de función para comparar claves y destruir datos. */
typedef int (*abb_comparar_clave_t)(const char*, const char*);
typedef void (*abb_destruir_dato_t)(void*);

typedef struct abb abb_t;
struct abb_iter;
typedef struct abb_iter abb_iter_t;

/* Primitivas del mapa. */

/* Crea el ABB */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el ABB, si la clave ya se encuentra en la estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura ABB fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t* arbol, const char* clave, void* dato);

/* Borra un elemento del ABB y devuelve el dato asociado. Devuelve NULL si el dato no estaba.
 * Pre: La estructura ABB fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió, en el caso de que estuviera guardado.
 */
void* abb_borrar(abb_t* arbol, const char* clave);

/* Obtiene el dato de un elemento del ABB, si la clave no se encuentra devuelve NULL.
 * Pre: La estructura ABB fue inicializada
 * Post: Devuelve el dato asociado a la clave ingresada, en el caso de que estuviera guardado.
 */
void* abb_obtener(const abb_t* arbol, const char* clave);

/* Determina si clave pertenece o no al ABB.
 * Pre: La estructura ABB fue inicializada
 */
bool abb_pertenece(const abb_t* arbol, const char* clave);

/* Devuelve la cantidad de elementos del ABB.
 * Pre: La estructura ABB fue inicializada
 */
size_t abb_cantidad(abb_t* arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función destruir para cada par (clave, dato).
 * Pre: La estructura ABB fue inicializada
 * Post: La estructura ABB fue destruida
 */
void abb_destruir(abb_t* arbol);

/* Implementa el iterador interno.
   "visitar" es una función de callback que recibe la clave, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario).
 */
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra);

/* Primitivas del iterador externo. */

/* Obtiene un iterador del ABB
 * Pre: el ABB fue creado
 * Post: se devuelve un nuevo iterador
 */
abb_iter_t* abb_iter_in_crear(const abb_t* arbol);

/* Avanza el iterador siguiendo in-order
 * Pre: el iterador fue creado
 * Post: se avanzó la posición actual en el iterador. Devuelve falso si está al final del ABB
 */
bool abb_iter_in_avanzar(abb_iter_t* iter);

/* Obtiene la clave del elemento que se encuentra en la posición actual
 * Pre: el iterador fue creado
 * Post: se almacena la clave del elemento actual. Devuelve NULL si está al final del ABB y por ende no puede ser guardado
 */
const char* abb_iter_in_ver_actual(const abb_iter_t* iter);

/* Verifica si ya está al final del ABB
 * Pre: el iterador fue creado
 * Post: devuelve verdadero si está al final del ABB y falso si no lo está
 */
bool abb_iter_in_al_final(const abb_iter_t* iter);

/* Destruye el iterador
 * Pre: El iterador fue inicializado
 * Post: El iterador fue destruido
 */
void abb_iter_in_destruir(abb_iter_t* iter);

#endif  // ABB_H
