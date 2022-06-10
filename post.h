#ifndef POST_H
#define POST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct post post_t;

/* Primitivas del post */

/* Crea un post con su texto y autor ya definidos.
 */
post_t* post_crear(char* texto, char* autor);

/* Destruye el post
 * Pre: El post fue inicializado
 * Post: El post fue destruido
 */
void post_destruir(post_t* post);

/* Agrega un like al post a nombre de quien lo likeó.
 * En caso que no pueda efectuarse el like devuelve false.
 * Pre: El post fue inicializado.
 * Post: Se likeó el post.
 */
bool post_likear(post_t* post, char* liker);

/* Devuelve el nombre del autor del post.
 * Pre: El post fue inicializado.
 */
char* post_autor(post_t* post);

/* Devuelve el texto del post.
 * Pre: El post fue inicializado.
 */
char* post_texto(post_t* post);

/* Devuelve en orden alfabético todos los likers del post.
 * Pre: El post fue inicializado.
 */
char* post_likes(post_t* post);

/* Devuelve la cantidad de likes del post.
 * Pre: El post fue inicializado.
 */
size_t post_cantidad_likes(post_t* post);

/* Determina si el post tiene likes o no.
 * En caso que no tenga likes devuelve false.
 * Pre: El post fue inicializado.
 */
bool post_tiene_likes(post_t* post);

#endif  // POST_H
