#ifndef TABLA_USUARIOS_H
#define TABLA_USUARIOS_H

#include <stdbool.h>
#include <stddef.h>

/* Primitivas de la tabla */

typedef struct tabla_usuarios tabla_usuarios_t;

/* Crea una tabla de usuarios a partir de un archivo externo.
 */
tabla_usuarios_t* tabla_usuarios_crear(char* nombre_archivo);

/* Destruye la tabla.
 * Pre: La tabla fue inicializada.
 * Post: La tabla fue destruida.
 */
void tabla_usuarios_destruir(tabla_usuarios_t* tabla_usuarios);

/* Guarda un usuario en la tabla.
 * En caso de no poder guardarlo devuelve false.
 * Pre: La tabla fue inicializada.
 * Post: Se almacenó el usuario.
 */
bool tabla_usuarios_guardar(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario);

/* Determina si un usuario se encuentra en la tabla.
 * En caso que no exista tal usuario en la tabla devuelve -1.
 * Pre: La tabla fue inicializada.
 */
bool tabla_usuarios_existe(tabla_usuarios_t* tabla_usuarios_t, char* nombre_usuario);

/* Borra un cierto usuario de la tabla.
 * En caso que no exista tal usuario devuelve false.
 * Pre: La tabla fue inicializada.
 * Post: Se borró el usuario.
 */
bool tabla_usuarios_borrar(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario);

/* Agrega un cierto post a los feeds de todos los usuarios de la tabla.
 * En caso que no pueda agregarse correctamente el post devuelve false.
 * Pre: La tabla fue inicializada.
 * Post: Se agregó el post en todos los feeds.
 */
bool tabla_usuarios_agregar_post(tabla_usuarios_t* tabla_usuarios, size_t id_autor, size_t id_post);

/* Devuelve el id del siguiente post a mostrar de un cierto usuario de la tabla.
 * En caso que no exista tal usuario en la tabla o que no hayan más posts en el feed devuelve -1.
 * Pre: La tabla fue inicializada.
 * Post: Se devolvió el id del siguiente post del feed.
 */
size_t tabla_usuarios_ver_siguiente_feed(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario);

/* Determina si el feed de un usuario se encuentra vacío.
 * En caso que no exista tal usuario en la tabla o que el feed esté vacío devuelve false.
 * Pre: La tabla fue inicializada.
 */
bool tabla_usuarios_feed_esta_vacio(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario);

/* Devuelve el id de un cierto usuario de la tabla.
 * En caso que no exista tal usuario en la tabla devuelve -1.
 * Pre: La tabla fue inicializada.
 */
size_t tabla_usuarios_id(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario);

#endif  // TABLA_USUARIOS_H
