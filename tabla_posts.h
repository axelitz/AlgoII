#ifndef TABLA_POSTS_H
#define TABLA_POSTS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct tabla_posts tabla_posts_t;

/* Primitivas de la tabla */

/* Crea una tabla de posts.
 */
tabla_posts_t* tabla_posts_crear();

/* Destruye la tabla.
 * Pre: La tabla fue inicializada.
 * Post: La tabla fue destruida.
 */
void tabla_posts_destruir(tabla_posts_t* tabla_posts);

/* Devuelve la cantidad de posts en la tabla.
 * Pre: La tabla fue inicializada.
 */
size_t tabla_posts_cantidad(tabla_posts_t* tabla_posts);

/* Guarda un post en la tabla.
 * En caso de no poder guardarlo devuelve false.
 * Pre: La tabla fue inicializada.
 * Post: Se almacenó el post.
 */
bool tabla_posts_guardar(tabla_posts_t* tabla_posts, char* texto, char* nombre_autor);

/* Determina si un post se encuentra en la tabla.
 * En caso que no exista tal post en la tabla devuelve false.
 * Pre: La tabla fue inicializada.
 */
bool tabla_posts_existe(tabla_posts_t* tabla_posts, size_t id_post);

/* Borra un cierto post de la tabla.
 * En caso que no exista tal post en la tabla devuelve false.
 * Pre: La tabla fue inicializada.
 * Post: Se borró el post.
 */
bool tabla_posts_borrar(tabla_posts_t* tabla_posts, size_t id_post);

/* Likea un cierto post de la tabla.
 * En caso que no exista tal post en la tabla o que no pueda efectuarse el like devuelve false.
 * Pre: La tabla fue inicializada.
 * Post: Se likeó el post.
 */
bool tabla_posts_likear(tabla_posts_t* tabla_posts, size_t id_post, char* nombre_liker);

/* Determina si un cierto post de la tabla tiene likes o no.
 * En caso que no exista tal post en la tabla o que no tenga likes devuelve false.
 * Pre: La tabla fue inicializada.
 */
bool tabla_posts_tiene_likes(tabla_posts_t* tabla_posts, size_t id_post);

/* Devuelve el nombre del autor de un cierto post de la tabla.
 * En caso que no exista tal post en la tabla devuelve NULL.
 * Pre: La tabla fue inicializada.
 * Post: Devolvió el nombre del autor del post.
 */
char* tabla_posts_autor(tabla_posts_t* tabla_posts, size_t id_post);

/* Devuelve el texto de un cierto post de la tabla.
 * En caso que no exista tal post en la tabla like devuelve NULL.
 * Pre: La tabla fue inicializada.
 * Post: Devolvió el texto del post.
 */
char* tabla_posts_texto(tabla_posts_t* tabla_posts, size_t id_post);

/* Devuelve en orden alfabético todos los likers de un cierto post de la tabla.
 * En caso que no exista tal post en la tabla o que no tenga likes devuelve NULL.
 * Pre: La tabla fue inicializada.
 * Post: Devolvió en orden alfabético todos los likers del post.
 */
char* tabla_posts_likes(tabla_posts_t* tabla_posts, size_t id_post);

/* Devuelve la cantidad de likes de un cierto post de la tabla.
 * En caso que no exista tal post en la tabla devuelve -1.
 * Pre: La tabla fue inicializada.
 * Post: Devolvió la cantidad de likes del post.
 */
size_t tabla_posts_cantidad_likes(tabla_posts_t* tabla_posts, size_t id_post);

#endif  // TABLA_POSTS_H
