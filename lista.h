#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

struct lista_iter;
typedef struct lista_iter lista_iter_t;
typedef struct lista lista_t;

/*
 * Primitivas de la lista
 */
 
// Crea una lista
// Post: Devuelve una nueva lista.
lista_t* lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos enlistados y falso en caso contrario.
// Pre: la lista fue creada
bool lista_esta_vacia(const lista_t* lista);

// Agrega un elemento al principio de la lista
// Pre: la lista fue creada
// Post: se agregó un nodo con el dato recibido al principio de la lista
bool lista_insertar_primero(lista_t* lista, void* dato);

// Agrega un elemento al final de la lista
// Pre: la lista fue creada
// Post: se agregó un nodo con el dato recibido al final de la lista
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Borra el primer elemento de la lista
// Pre: la lista fue creada
// Post: se eliminó el primer nodo de la lista y se devuelve el dato contenido. En caso de que la lista esté vacía devuelve NULL.
void* lista_borrar_primero(lista_t* lista);

// Devuelve el contenido de dato del primer nodo
// Pre: la lista fue creada
// Post: devuelve el valor almacenado en el primer dato. En caso de que la lista esté vacía devuelve NULL.
void* lista_ver_primero(const lista_t* lista);

// Devuelve el contenido de dato del ultimo nodo
// Pre: la lista fue creada
// Post: devuelve el valor almacenado en el ultimo dato. En caso de que la lista esté vacía devuelve NULL.
void* lista_ver_ultimo(const lista_t* lista);

// Obtener largo de la lista
// Pre: la lista fue creada
// Post: devuelve la cantidad de elementos en la lista
size_t lista_largo(const lista_t* lista);

// Destruye la lista
void lista_destruir(lista_t* lista, void (*destruir_dato)(void*));

/*
 * Primitivas de iterador interno
 */

// Iterador interno de la lista.
// Recorre la lista y para cada elemento de la misma llama a la función visitar, pasándole por parámetro el elemento de la lista
// (dato) y el parámetro extra. El parámetro extra sirve para introducir información adicional en la función visitar. Este dato ser NULL.
// La función visitar debe devolver verdadero si se debe seguir iterando y falso para cortar la iteración. No puede agregar ni quitar elementos a la lista.
// Pre: la lista fue creada.
// Post: se llamó a la función visitar una vez por cada elemento de la lista de forma ordenada.
void lista_iterar(lista_t* lista, bool (*visitar)(void* dato, void* extra), void* extra);

/*
 * Primitivas del iterador externo
 */

// Obtiene un iterador de la lista
// Pre: la lista fue creada
// Post: se devuelve un nuevo iterador
lista_iter_t* lista_iter_crear(lista_t* lista);

// Avanza una posición en la lista
// Pre: el iterador fue creado
// Post: se avanzó la posición actual en el iterador. Devuelve falso si está al final de la lista
bool lista_iter_avanzar(lista_iter_t* iter);

// Almacena en dato el valor que se encuentra en la posición actual
// Pre: el iterador fue creado
// Post: se almacenó el dato actual en dato. Devuelve NULL si está al final de la lista y por ende no puede ser guardado
void* lista_iter_ver_actual(const lista_iter_t* iter);

// Verifica si ya está al final de la lista
// Pre: el iterador fue creado
// Post: devuelve verdadero si está al final de la lista y falso si no lo está
bool lista_iter_al_final(const lista_iter_t* iter);

// Destruye el iterador de la lista
void lista_iter_destruir(lista_iter_t* iter);

// Agrega un elemento en la posición actual
// Pre: el iterador fue creado
// Post: se insertó un nuevo nodo antes de la posición actual, quedando este nuevo nodo como posición actual. Devuelve falso si el nodo no pudo ser insertado
bool lista_iter_insertar(lista_iter_t* iter, void* dato);

// Elimina el elemento de la posición actual
// Pre: el iterador fue creado
// Post: se eliminó el nodo que se encontraba en la posición actual indicada por el iterador. Devuelve NULL si el iterador está al final de la lista y por ende no tiene qué borrar
void* lista_iter_borrar(lista_iter_t* iter);

#endif // LISTA_H
