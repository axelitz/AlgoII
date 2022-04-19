#include <stdlib.h>
#include <stdio.h>

FILE* abrir_archivo(char* nombre_archivo, char* modo_archivo) {
	return fopen(nombre_archivo, modo_archivo);
}

void cerrar_archivo(FILE* archivo) {
	fclose(archivo);
}

void dividir_archivo(int cantidad_columnas, FILE* archivo_lectura) {
	int caracter_leido;
	int columnas = 0;
	while ((caracter_leido = fgetc(archivo_lectura)) != EOF) {
		fputc(caracter_leido, stdout);
		columnas++;
		if (columnas == cantidad_columnas) fputc('\n', stdout);
		if (caracter_leido == '\n' || columnas == cantidad_columnas) columnas = 0;
	}
}

int validar_cantidad_columnas(int argc, char* argv []) {
	int cantidad_columnas = 0;

	if (argc < 2 || argc > 3 || (cantidad_columnas = atoi(argv[1])) == 0) fprintf(stderr, "ERROR: Cantidad erronea de parametros\n");
	
	return cantidad_columnas;
}

FILE* validar_archivo_lectura(int argc, char* argv[]) {
	FILE* archivo_lectura;
	
	if (argc == 2) archivo_lectura = stdin;
	else if ((archivo_lectura = abrir_archivo(argv[2], "r")) == NULL) fprintf(stderr, "Error: archivo fuente inaccesible\n");
	
	return archivo_lectura;
}

int main(int argc, char* argv []) {
	int cant_columnas = validar_cantidad_columnas(argc, argv);
	if (cant_columnas == 0) return -1;
	
	FILE* archivo_lectura = validar_archivo_lectura(argc, argv);
	if (archivo_lectura == NULL) return -1;
	
	dividir_archivo(cant_columnas, archivo_lectura);
	cerrar_archivo(archivo_lectura);
	return 0;
}
