#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabla_usuarios.h"
#include "hash.h"
#include "usuario.h"

struct tabla_usuarios {
	hash_t* hash;
};

/* *****************************************************************
 *                	  FUNCIONES AUXILIARES
 * *****************************************************************/

char* obtener_getline(FILE* archivo_lectura) {
	size_t numero_bytes = 0;
	char* string = NULL;
	if (getline(&string, &numero_bytes, archivo_lectura) < 0) {
		free(string);
		return NULL;
	}
	string[strlen(string)-1]='\0';
	return string;
}

void liberar_string_de_getline(char** string) {
	if (*string != NULL) {
		free(*string);
		*string = NULL;
	}
}

void destruir_usuario (void* usuario) {
	usuario_destruir((usuario_t*)usuario);
}

bool tabla_usuarios_guardar_desde_archivo (tabla_usuarios_t* tabla_usuarios, char* nombre_archivo) {
	FILE* archivo_lectura;
	if ((archivo_lectura = fopen(nombre_archivo, "r")) == NULL) return false;

	char* nombre_usuario;

	while ((nombre_usuario = obtener_getline(archivo_lectura)) != NULL) {
		if (!tabla_usuarios_guardar(tabla_usuarios, nombre_usuario)) return false;
		liberar_string_de_getline(&nombre_usuario);
	}

	fclose(archivo_lectura);
	return true;
}

usuario_t* tabla_usuarios_obtener_usuario(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	return tabla_usuarios_existe(tabla_usuarios, nombre_usuario) ? (usuario_t*)hash_obtener(tabla_usuarios->hash, nombre_usuario) : NULL;
}

usuario_t* obtener_usuario_desde_iterador(hash_iter_t* iter, tabla_usuarios_t* tabla_usuarios) {
	const char* nombre_usuario = hash_iter_ver_actual(iter);
	usuario_t* usuario = (usuario_t*)hash_obtener(tabla_usuarios->hash, nombre_usuario);
	return usuario;
}

void vaciar_tabla_de_usuarios(tabla_usuarios_t* tabla_usuarios, size_t id_post) {
	hash_iter_t* iter = hash_iter_crear(tabla_usuarios->hash);
	while (!hash_iter_al_final(iter)) {
		usuario_t* usuario = obtener_usuario_desde_iterador(iter, tabla_usuarios);
		usuario_eliminar_post(usuario, id_post);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
}

/* *****************************************************************
 *                PRIMITIVAS DE LA TABLA DE USUARIOS
 * *****************************************************************/

tabla_usuarios_t* tabla_usuarios_crear(char* nombre_archivo) {
	tabla_usuarios_t* tabla_usuarios = malloc(sizeof(tabla_usuarios_t));
	if (tabla_usuarios == NULL) return NULL;

	tabla_usuarios->hash = hash_crear(destruir_usuario);
	if (tabla_usuarios->hash == NULL) {
		free(tabla_usuarios);
		return NULL;
	}

	if (!tabla_usuarios_guardar_desde_archivo(tabla_usuarios, nombre_archivo)) {
		hash_destruir(tabla_usuarios->hash);
		free(tabla_usuarios);
		return NULL;
	}

	return tabla_usuarios;
}

void tabla_usuarios_destruir(tabla_usuarios_t* tabla_usuarios) {
	hash_destruir(tabla_usuarios->hash);
	free(tabla_usuarios);
}

bool tabla_usuarios_guardar(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	size_t id_usuario = hash_cantidad(tabla_usuarios->hash);
	usuario_t* usuario;
	if ((usuario = usuario_crear(id_usuario)) == NULL) return false;

	if (!hash_guardar(tabla_usuarios->hash, nombre_usuario, usuario)) {
		usuario_destruir(usuario);
		return false;
	}

	return true;
}

bool tabla_usuarios_existe(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	return hash_pertenece(tabla_usuarios->hash, nombre_usuario);
}

bool tabla_usuarios_borrar(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	if (!tabla_usuarios_existe(tabla_usuarios, nombre_usuario)) return false;

	usuario_t* usuario = (usuario_t*)hash_borrar(tabla_usuarios->hash, nombre_usuario);
	usuario_destruir(usuario);
	return true;
}
 
bool tabla_usuarios_agregar_post(tabla_usuarios_t* tabla_usuarios, size_t id_autor, size_t id_post) {
	hash_iter_t* iter;
	if ((iter = hash_iter_crear(tabla_usuarios->hash)) == NULL) return false;

	while (!hash_iter_al_final(iter)) {
		usuario_t* usuario = obtener_usuario_desde_iterador(iter, tabla_usuarios);
		if (!usuario_agregar_post(usuario, id_autor, id_post)) {
			vaciar_tabla_de_usuarios(tabla_usuarios, id_post);
			hash_iter_destruir(iter);
			return false;
		}
		hash_iter_avanzar(iter);
	}
	
	hash_iter_destruir(iter);
	return true;
}

size_t tabla_usuarios_ver_siguiente_feed(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	usuario_t* usuario;
	return (usuario = tabla_usuarios_obtener_usuario(tabla_usuarios, nombre_usuario)) != NULL ? usuario_ver_siguiente_feed(usuario) : -1;
}

bool tabla_usuarios_feed_esta_vacio(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	usuario_t* usuario;
	return (usuario = tabla_usuarios_obtener_usuario(tabla_usuarios, nombre_usuario)) != NULL ? usuario_feed_esta_vacio(usuario) : false;
}

size_t tabla_usuarios_id(tabla_usuarios_t* tabla_usuarios, char* nombre_usuario) {
	usuario_t* usuario;
	return (usuario = tabla_usuarios_obtener_usuario(tabla_usuarios, nombre_usuario)) != NULL ? usuario_id(usuario) : -1;
}
