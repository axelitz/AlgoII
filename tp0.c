#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
	aux = *x;
	*x = *y;
	*y = aux;
}


int maximo(int vector[], int n) {
	index_max = -1;
	if (n > 0) {
		max = vector[0];
		index_max = 0;
		for (i = 1; i < n; i++){
			if (vector[i] > max) {
				max = vector[i];
				index_max = i; 
			}
		}
	}
	return index_max;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    return 0;
}


void seleccion(int vector[], int n) {
}
