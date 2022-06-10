#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usuario.h"
#include "heap.h"

struct usuario {
	size_t id;
	heap_t* feed;
};

// Estructura para ordenar los posts en el feed
typedef struct post_feed {
	int afinidad;
	size_t id_post;
} post_feed_t;

/* *****************************************************************
 *                     FUNCIONES AUXILIARES
 * *****************************************************************/

int feed(const void* a, const void* b) {
	post_feed_t* post_a = (post_feed_t*)a;
	post_feed_t* post_b = (post_feed_t*)b;
	if(post_a->afinidad != post_b->afinidad) return post_a->afinidad < post_b->afinidad ? 1 : -1;
	return post_a->id_post < post_b->id_post ? 1 : -1;
}

/* *****************************************************************
 *                     PRIMITIVAS DEL USUARIO
 * *****************************************************************/

usuario_t* usuario_crear(size_t id) {
	usuario_t* usuario = malloc(sizeof(usuario_t));
	if (usuario == NULL) return NULL;
	
	usuario->id = id;
	usuario->feed = heap_crear(feed);
	if(usuario->feed == NULL) {
		free(usuario);
		return NULL;
	}

	return usuario;
}

void usuario_destruir(usuario_t* usuario) {
	heap_destruir(usuario->feed, free);
	free(usuario);
}

bool usuario_feed_esta_vacio(usuario_t* usuario) {
	return heap_esta_vacio(usuario->feed);
}

bool usuario_agregar_post(usuario_t* usuario, size_t id_autor, size_t id_post) {
	if (id_autor == usuario->id) return true;

	post_feed_t* post_feed = malloc(sizeof(post_feed_t));
	if (post_feed == NULL) return false;

	post_feed->afinidad = abs((int)id_autor - (int)usuario->id);
	post_feed->id_post = id_post;
	
	return heap_encolar(usuario->feed, post_feed);
}

bool usuario_eliminar_post(usuario_t* usuario, size_t id_post) {
	if (usuario_feed_esta_vacio(usuario)) return false;

	post_feed_t* post_feed = (post_feed_t*)heap_ver_max(usuario->feed);
	if (post_feed->id_post == id_post) return true;

	post_feed = (post_feed_t*)heap_desencolar(usuario->feed);
	bool siguiente = usuario_eliminar_post(usuario, id_post);
	heap_encolar(usuario->feed, post_feed);
	return siguiente;
}

size_t usuario_ver_siguiente_feed(usuario_t* usuario) {
	if (usuario_feed_esta_vacio(usuario)) return -1;

	post_feed_t* siguiente_post = (post_feed_t*)heap_desencolar(usuario->feed);
	size_t id_post = siguiente_post->id_post;
	free(siguiente_post);
	return id_post;
}

size_t usuario_id(usuario_t* usuario) {
	return usuario->id;
}
