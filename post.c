#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "post.h"
#include "abb.h"

struct post {
	char* autor;
	char* texto;
	abb_t* likes;
};

/* *****************************************************************
 *                	  FUNCIONES AUXILIARES
 * *****************************************************************/

void cats(char** str, const char* str2) {
	char* tmp = NULL;

	if (*str == NULL) {
		*str = calloc(strlen(str2)+1, sizeof(char));
		memcpy(*str, str2, strlen(str2));
	}
	else {
		tmp = calloc(strlen(*str)+1, sizeof(char));
		memcpy(tmp, *str, strlen(*str));
		size_t len = strlen(*str);
		free(*str);
		*str = calloc(len+strlen(str2)+1, sizeof(char));
		memcpy(*str, tmp, strlen(tmp));
		memcpy(*str + strlen(*str), str2, strlen(str2));
		free(tmp);
	}
}

bool imprimir_likes(const char* clave, void* dato, void* likes) {
	cats((char**)likes, "	");
	cats((char**)likes, clave);
	cats((char**)likes, "\n");
	return true;
}

/* *****************************************************************
 *                     PRIMITIVAS DEL POST
 * *****************************************************************/

post_t* post_crear(char* texto, char* autor) {
	post_t* post = malloc(sizeof(post_t));
	if (post == NULL) return NULL;

	post->autor = autor;
	post->texto = texto;
	post->likes = abb_crear(strcmp, NULL);
	if (post->likes == NULL) {
		free(post->texto);
		free(post->autor);
		free(post);
		return NULL;
	}

	return post;
}

void post_destruir(post_t* post) {
	abb_destruir(post->likes);
	free(post->texto);
	free(post->autor);
	free(post);
}

bool post_likear(post_t* post, char* liker) {
	return abb_guardar(post->likes, liker, NULL);
}

char* post_autor(post_t* post) {
	return post->autor;
}

char* post_texto(post_t* post) {
	return post->texto;
}

char* post_likes(post_t* post) {
	char* likes = NULL;
	abb_in_order(post->likes, imprimir_likes, &likes);
	if (likes != NULL) likes[strlen(likes)-1]='\0';
	return likes;
}

size_t post_cantidad_likes(post_t* post) {
	return abb_cantidad(post->likes);
}

bool post_tiene_likes(post_t* post) {
	return post_cantidad_likes(post);
}
