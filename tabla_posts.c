#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabla_posts.h"
#include "post.h"
#include "hash.h"

#define CANTIDAD_MAXIMA 20

struct tabla_posts {
	hash_t* hash;
};

/* *****************************************************************
 *                	  FUNCIONES AUXILIARES
 * *****************************************************************/

void destruir_post (void* post) {
	post_destruir((post_t*)post);
}

char* obtener_clave_segun_id_post(size_t id_post) {
	const size_t largo_clave = 10;
	char* clave_post = malloc(CANTIDAD_MAXIMA*largo_clave);
	sprintf(clave_post, "%08ld", id_post);
	return clave_post;
}

post_t* tabla_posts_obtener_post(tabla_posts_t* tabla_posts, size_t id_post) {
	char* clave_post = obtener_clave_segun_id_post(id_post);
	post_t* post = tabla_posts_existe(tabla_posts, id_post) ? (post_t*)hash_obtener(tabla_posts->hash, clave_post) : NULL;
	free(clave_post);
	return post;
}

/* *****************************************************************
 *                PRIMITIVAS DE LA TABLA DE POSTS
 * *****************************************************************/

tabla_posts_t* tabla_posts_crear() {
	tabla_posts_t* tabla_posts = malloc(sizeof(tabla_posts_t));
	if (tabla_posts == NULL) return NULL;

	tabla_posts->hash = hash_crear(destruir_post);
	if (tabla_posts->hash == NULL) {
		free(tabla_posts);
		return NULL;
	}

	return tabla_posts;
}

void tabla_posts_destruir(tabla_posts_t* tabla_posts) {
	hash_destruir(tabla_posts->hash);
	free(tabla_posts);
}

size_t tabla_posts_cantidad(tabla_posts_t* tabla_posts) {
	return hash_cantidad(tabla_posts->hash);
}

bool tabla_posts_guardar(tabla_posts_t* tabla_posts, char* texto, char* nombre_autor) {
	post_t* post;
	if ((post = post_crear(texto, nombre_autor)) == NULL) return false;

	size_t id_post = tabla_posts_cantidad(tabla_posts);
	char* clave_post = obtener_clave_segun_id_post(id_post);
	if (!hash_guardar(tabla_posts->hash, clave_post, post)) {
		free(clave_post);
		post_destruir(post);
		return false;
	}

	free(clave_post);
	return true;
}

bool tabla_posts_existe(tabla_posts_t* tabla_posts, size_t id_post) {
	char* clave_post = obtener_clave_segun_id_post(id_post);
	bool existe = hash_pertenece(tabla_posts->hash, clave_post);
	free(clave_post);
	return existe;
}

bool tabla_posts_borrar(tabla_posts_t* tabla_posts, size_t id_post) {
	if (!tabla_posts_existe(tabla_posts, id_post)) return false;
	char* clave_post = obtener_clave_segun_id_post(id_post);
	post_t* post = (post_t*)hash_borrar(tabla_posts->hash, clave_post);
	free(clave_post);
	post_destruir(post);
	return true;
}

bool tabla_posts_likear(tabla_posts_t* tabla_posts, size_t id_post, char* nombre_liker) {
	post_t* post_a_likear = tabla_posts_obtener_post(tabla_posts, id_post);
	return post_a_likear != NULL ? post_likear(post_a_likear, nombre_liker) : false;
}

bool tabla_posts_tiene_likes(tabla_posts_t* tabla_posts, size_t id_post) {
	post_t* post_a_verle_los_likes = tabla_posts_obtener_post(tabla_posts, id_post);
	return post_a_verle_los_likes != NULL ? post_tiene_likes(post_a_verle_los_likes) : false;
}

char* tabla_posts_autor(tabla_posts_t* tabla_posts, size_t id_post) {
	post_t* post_a_verle_el_autor = tabla_posts_obtener_post(tabla_posts, id_post);
	if(post_a_verle_el_autor == NULL) printf ("ACÁ ESTÁ EL ERROR");
	return post_a_verle_el_autor != NULL ? post_autor(post_a_verle_el_autor) : NULL;
}

char* tabla_posts_texto(tabla_posts_t* tabla_posts, size_t id_post) {
	post_t* post_a_verle_el_texto = tabla_posts_obtener_post(tabla_posts, id_post);
	return post_a_verle_el_texto != NULL ? post_texto(post_a_verle_el_texto) : NULL;
}

char* tabla_posts_likes(tabla_posts_t* tabla_posts, size_t id_post) {
	post_t* post_a_verle_los_likes = tabla_posts_obtener_post(tabla_posts, id_post);
	return post_a_verle_los_likes != NULL ? post_likes(post_a_verle_los_likes) : NULL;
}

size_t tabla_posts_cantidad_likes(tabla_posts_t* tabla_posts, size_t id_post) {
	post_t* post_a_verle_la_cantidad_de_likes = tabla_posts_obtener_post(tabla_posts, id_post);
	return post_a_verle_la_cantidad_de_likes != NULL ? post_cantidad_likes(post_a_verle_la_cantidad_de_likes) : -1;
}
