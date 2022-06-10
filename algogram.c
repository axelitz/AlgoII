#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tabla_usuarios.h"
#include "tabla_posts.h"

/* *****************************************************************
 *                	  FUNCIONES AUXILIARES
 * *****************************************************************/

char* obtener_getline(FILE* archivo_lectura);
void liberar_string_de_getline(char** string);

size_t obtener_scanf() {
	size_t num;
	while (scanf("%lu", &num) != 1) fprintf (stderr, "Error: ingrese nuevamente el número\n");
	return num;
}

char* duplicar_string(const char* org) {
	char* cpy = malloc(sizeof(char) * (strlen(org) + 1));
	if (!cpy) return NULL;
	strcpy(cpy, org);
	return cpy;
}

/* *****************************************************************
 *                	FUNCIONES PRINCIPALES
 * *****************************************************************/

void login(bool* logueado, char** usuario_actual, tabla_usuarios_t* tabla_usuarios) {
	char* usuario_login = obtener_getline(stdin);

	if (*logueado) {
		printf("Error: Ya habia un usuario loggeado\n");
		free(usuario_login);
		return;
	}
	if (!tabla_usuarios_existe(tabla_usuarios, usuario_login)) {
		printf("Error: usuario no existente\n");
		free(usuario_login);
		return;
	}

	printf("Hola %s\n", usuario_login);
	*usuario_actual = usuario_login;
	*logueado = true;
}

void logout(bool* logueado, char** usuario_actual) {
	if (!*logueado) {
		printf ("Error: no habia usuario loggeado\n");
		return;
	}

	free(*usuario_actual);
	*usuario_actual = NULL;
	printf("Adios\n");
	*logueado = false;
}

void publicar(bool logueado, char* usuario_actual, tabla_usuarios_t* tabla_usuarios, tabla_posts_t* tabla_posts) {
	char* texto = obtener_getline(stdin);

	if (!logueado) {
		printf("Error: no habia usuario loggeado\n");
		free(texto);
		return;
	}
	
	char* autor;
	if ((autor = duplicar_string(usuario_actual)) == NULL) {
		fprintf(stderr, "Error: no se pudo realizar la publicación\n");
		free(texto);
		return;
	}
	if (!tabla_posts_guardar(tabla_posts, texto, autor)) {
		fprintf(stderr, "Error: no se pudo realizar la publicación\n");
		free(autor);
		free(texto);
		return;
	}
	
	size_t id_autor = tabla_usuarios_id(tabla_usuarios, usuario_actual);
	size_t id_post = tabla_posts_cantidad(tabla_posts)-1;
	if (!tabla_usuarios_agregar_post(tabla_usuarios, id_autor, id_post)) {
		fprintf(stderr, "Error: no se pudo realizar la publicación\n");
		tabla_posts_borrar(tabla_posts, id_post);
		free(autor);
		free(texto);
		return;
	}
	printf("Post publicado\n");
}

void ver_siguiente_feed(bool logueado, char* usuario_actual, tabla_usuarios_t* tabla_usuarios, tabla_posts_t* tabla_posts) {
	if (!logueado || tabla_usuarios_feed_esta_vacio(tabla_usuarios, usuario_actual)) {
		printf("Usuario no loggeado o no hay mas posts para ver\n");
		return;
	}

	size_t id_post = tabla_usuarios_ver_siguiente_feed(tabla_usuarios, usuario_actual);
	char* autor = tabla_posts_autor(tabla_posts, id_post);
	char* texto = tabla_posts_texto(tabla_posts, id_post);
	size_t cantidad_likes = tabla_posts_cantidad_likes(tabla_posts, id_post);
	printf("Post ID %ld\n", id_post);
	printf("%s dijo: %s\n", autor, texto);
	printf("Likes: %ld\n", cantidad_likes);
}

void likear_post(bool logueado, char* usuario_actual, tabla_posts_t* tabla_posts) {
	size_t id_post = obtener_scanf();

	if(!logueado || !tabla_posts_existe(tabla_posts, id_post)) {
		printf("Error: Usuario no loggeado o Post inexistente\n");
		return;
	}
	
	if (tabla_posts_likear(tabla_posts, id_post, usuario_actual)) printf("Post likeado\n");
	else fprintf (stderr, "Error: El post no pudo ser likeado\n");
}

void mostrar_likes(tabla_posts_t* tabla_posts) {
	size_t id_post = obtener_scanf();

	if(!tabla_posts_existe(tabla_posts, id_post) || !tabla_posts_tiene_likes(tabla_posts, id_post)) {
		printf("Error: Post inexistente o sin likes\n");
		return;
	}

	char* likes;
	if ((likes = tabla_posts_likes(tabla_posts, id_post)) == NULL) {
		fprintf (stderr, "Error: No se pudieron obtener los likes del post\n");
		return;
	}
	size_t cantidad_likes = tabla_posts_cantidad_likes(tabla_posts, id_post);

	printf("El post tiene %ld likes:\n", cantidad_likes);
	printf("%s\n", likes);
	free(likes);
}
