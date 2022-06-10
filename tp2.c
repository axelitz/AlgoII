#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tabla_usuarios.h"
#include "tabla_posts.h"

#define LOGIN 1
#define LOGOUT 2
#define PUBLICAR 3
#define VER_SIGUIENTE_FEED 4
#define LIKEAR_POST 5
#define MOSTRAR_LIKES 6
#define BADKEY -1

/* *****************************************************************
 *                	  FUNCIONES AUXILIARES
 * *****************************************************************/

char* obtener_getline(FILE* archivo_lectura);
void liberar_string_de_getline(char** string);

void login(bool* logueado, char** usuario_actual, tabla_usuarios_t* tabla_usuarios);
void logout(bool* logueado, char** usuario_actual);
void publicar(bool logueado, char* usuario_actual, tabla_usuarios_t* tabla_usuarios, tabla_posts_t* tabla_posts);
void ver_siguiente_feed(bool logueado, char* usuario_actual, tabla_usuarios_t* tabla_usuarios, tabla_posts_t* tabla_posts);
void likear_post(bool logueado, char* usuario_actual, tabla_posts_t* tabla_posts);
void mostrar_likes(tabla_posts_t* tabla_posts);

// Estructura para el switch del main
typedef struct clave_switch {
	char* clave;
	int valor;
} clave_switch_t;

// Diccionario para el switch del main
static clave_switch_t tabla_valores[] = { {"login", LOGIN}, {"logout", LOGOUT}, {"publicar", PUBLICAR}, {"ver_siguiente_feed", VER_SIGUIENTE_FEED}, {"likear_post", LIKEAR_POST}, {"mostrar_likes", MOSTRAR_LIKES} };

#define NRO_CLAVES (sizeof(tabla_valores)/sizeof(clave_switch_t))

int valor_desde_clave(char* clave) {
	int i;
	for (i = 0; i < NRO_CLAVES; i++) {
		clave_switch_t clave_switch = tabla_valores[i];
		if (strcmp(clave_switch.clave, clave) == 0) return clave_switch.valor;
    	}
	return BADKEY;
}

/* *****************************************************************
 *                	  	MAIN
 * *****************************************************************/

int main(int argc, char* argv[]) {
	tabla_usuarios_t* tabla_usuarios;
	if (argc != 2 || (tabla_usuarios = tabla_usuarios_crear(argv[1])) == NULL) {
		fprintf(stderr, "Error: no se pudo leer correctamente el archivo de usuarios\n");
		return -1;
	}

	tabla_posts_t* tabla_posts;
	if ((tabla_posts = tabla_posts_crear()) == NULL)  {
		fprintf(stderr, "Error: no se pudo inicializar correctamente Algogram\n");
		tabla_usuarios_destruir(tabla_usuarios);
		return -1;
	}

	bool logueado = false;
	char* usuario_actual = NULL;
	char* comando = NULL;

	while ((comando = obtener_getline(stdin)) != NULL) {
		if(strlen(comando) > 0) switch (valor_desde_clave(comando)) {
			case LOGIN:
				login(&logueado, &usuario_actual, tabla_usuarios);
				break;
			case LOGOUT:
				logout(&logueado, &usuario_actual);
				break;
			case PUBLICAR:
				publicar(logueado, usuario_actual, tabla_usuarios, tabla_posts);
				break;
			case VER_SIGUIENTE_FEED:
				ver_siguiente_feed(logueado, usuario_actual, tabla_usuarios, tabla_posts);
				break;
			case LIKEAR_POST:
				likear_post(logueado, usuario_actual, tabla_posts);
				break;
			case MOSTRAR_LIKES:
				mostrar_likes(tabla_posts);
				break;
			case BADKEY:
				fprintf(stderr, "Error: comando no válido\n");
				break;
		}
		liberar_string_de_getline(&comando);
	}

	if (usuario_actual != NULL) free(usuario_actual);
	tabla_posts_destruir(tabla_posts);
	tabla_usuarios_destruir(tabla_usuarios);
	return 0;
}
