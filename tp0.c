#include "tp0.h"
#include <stdio.h>

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
	int aux = *x;
	*x = *y;
	*y = aux;
}

int maximo(int vector[], int n) {
	if (n == 0) {
		return -1;
	}
	if (n == 1) {
		return 0;
	}
	int index_max = maximo(vector, n-1);
    	if (vector[index_max] < vector[n-1]) {
        	return n-1;
    	}
    	else {
        	return index_max;
    	}
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
	if (n1 > n2 && n2 == 0){
		return 1;
	}
	if (n1 < n2 && n1 == 0){
		return -1;
	}
	if (n1 == n2 && n1 == 0) {
		return 0;
	}
	if (vector1[0] > vector2[0]){
		return 1;
	}
	if (vector1[0] < vector2[0]){
		return -1;
	}
	return comparar(&vector1[1],n1-1,&vector2[1],n2-1);
}


void seleccion(int vector[], int n) {
	if (n > 1) {
		swap(&vector[maximo(vector, n)], &vector[n-1]);
		seleccion(vector, n-1);
	}
}
