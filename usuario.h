#ifndef USUARIO_H
#define USUARIO_H

#include <stdbool.h>
#include <stddef.h>

typedef struct usuario usuario_t;

/* Crea un usuario con su id ya definido.
 */
usuario_t* usuario_crear(size_t id);

/* Destruye el usuario.
 * Pre: El usuario fue inicializado.
 * Post: El usuario fue destruido.
 */
void usuario_destruir(usuario_t* usuario);

/* Determina si el feed del usuario se encuentra vacío.
 * En caso que el feed esté vacío devuelve false.
 * Pre: El usuario fue inicializado.
 */
bool usuario_feed_esta_vacio(usuario_t* usuario);

/* Agrega un post al feed del usuario.
 * En caso que no pueda agregarse correctamente el post devuelve false.
 * Pre: El usuario fue inicializado.
 * Post: Se agregó el post al feed.
 */
bool usuario_agregar_post(usuario_t* usuario, size_t id_autor, size_t id_post);

/* Borra un cierto post del feed del usuario.
 * En caso que no se encuentre el post devuelve false.
 * Pre: El usuario fue inicializado.
 * Post: Se borró el post al feed.
 */
bool usuario_eliminar_post(usuario_t* usuario, size_t id_post);

/* Devuelve el id del siguiente post a mostrar del usuario.
 * En caso que no hayan más posts en el feed devuelve -1.
 * Pre: El usuario fue inicializado.
 * Post: Se devolvió el id del siguiente post del feed.
 */
size_t usuario_ver_siguiente_feed(usuario_t* usuario);

/* Devuelve el id del usuario.
 * Pre: El usuario fue inicializado.
 */
size_t usuario_id(usuario_t* usuario);

#endif  // USUARIO_H
