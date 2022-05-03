#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int es_numero(char *n) {
  size_t i = strlen(n)-1;
  while (i && isdigit(n[i])) i--;
  return i == 0 ? 1 : 0;
}

void dividir_archivo(int cantidad_columnas, FILE* archivo_lectura) {
	int caracter_leido;
	bool salto_de_linea = false;
	int columnas = 0;
	while ((caracter_leido = fgetc(archivo_lectura)) != EOF) {
		if (columnas == 0 && caracter_leido == '\n') {
			if (!salto_de_linea) fputc(caracter_leido, stdout);
			else salto_de_linea = false;
		}
		else {
			columnas++;
			fputc(caracter_leido, stdout);
		}
		if (columnas == cantidad_columnas && caracter_leido != '\n') {
			fputc('\n', stdout);
			salto_de_linea = true;
		}
		if (columnas == cantidad_columnas || caracter_leido == '\n') columnas = 0;
	}
}

int validar_cantidad_parametros(int argc, char* argv []) {
	if (argc < 2 || argc > 3 || es_numero(argv[1]) == 0 || atoi(argv[1]) < 1) {
		fprintf(stderr, "Error: Cantidad erronea de parametros\n");
		return 0;
	}
	return atoi(argv[1]);
}

FILE* validar_archivo_lectura(int argc, char* argv[]) {
	FILE* archivo_lectura;
	
	if (argc == 2) archivo_lectura = stdin;
	else if ((archivo_lectura = fopen(argv[2], "r")) == NULL) fprintf(stderr, "Error: archivo fuente inaccesible\n");
	
	return archivo_lectura;
}

int main(int argc, char* argv []) {
	int cant_columnas = validar_cantidad_parametros(argc, argv);
	if (cant_columnas == 0) return -1;
	
	FILE* archivo_lectura = validar_archivo_lectura(argc, argv);
	if (archivo_lectura == NULL) return -1;
	
	dividir_archivo(cant_columnas, archivo_lectura);
	fclose(archivo_lectura);
	return 0;
}
