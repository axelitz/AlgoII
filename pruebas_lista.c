#include "lista.h"
#include "cola.h"
#include "pila.h"
#include "vector.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>


/* *****************************************************************
 *               	FUNCIONES AUXILIARES
 * *****************************************************************/

void destruir_cola(void* cola) {
	cola_destruir(cola, NULL);
}

void destruir_pila(void* pila) {
	pila_destruir(pila);
}

void destruir_vector_TDA(void* vector) {
	vector_destruir(vector);
}

bool sumar_elementos_de_lista(void* elemento, void* suma) {	
	*(int*)suma += *(int*)elemento;
	return true;
}

bool verificar_mayor_que_5(void* elemento, void* guardo_elemento) {
	if (*(int*)elemento > 5) {
		*(int*)guardo_elemento = *(int*)elemento;
		return false;
	}
	return true;
}

bool duplicar_elementos_de_lista(void* elemento, void* dato) {	
	*(int*)elemento *= 2;
	return true;
}

bool obtener_primer_elemento_iter_interno(void* dato, void* extra) {
	(*(int*)extra) = (*(int*)dato);
	return false;
}

bool obtener_ultimo_elemento_iter_interno(void* dato, void* extra) {
	(*(int*)extra) = (*(int*)dato);
	return true;
}

bool obtener_elementos_iter_interno(void* dato, void* extra) {
	int* elementos_obtenidos = (int*)extra;
	int* i = &elementos_obtenidos[10];
	
	(elementos_obtenidos[*i]) = (*(int*)dato);
	(*i)++;
	
	return true;
}

bool sumar_elementos_iter_interno(void* dato, void* extra) {
	int* aux = (int*)extra;
	int* acumulador = &aux[0];
	
	(*acumulador) += (*(int*)dato);
		
	return true;
}

bool sumar_primeros_x_elementos_iter_interno(void* dato, void* extra) {
	int* aux = (int*)extra;
	int* cant_elementos = &aux[0];
	int* contador = &aux[1];
	int* acumulador = &aux[2];
	
	if (*contador >= *cant_elementos) return false;
	(*contador)++;
	(*acumulador) += (*(int*)dato);
	
	return true;
}

bool factorial_primeros_x_elementos_iter_interno(void* dato, void* extra) {
	int* aux = (int*)extra;
	int* factorial = &aux[0];
	int* contador = &aux[1];
	int* acumulador = &aux[2];
	
	if (*contador >= *factorial) return false;
	(*contador)++;
	(*acumulador) = (*acumulador) * (*(int*)dato);
	
	return true;
}

/* *****************************************************************
 *               	FUNCIONES DE PRUEBA
 * *****************************************************************/

static void prueba_crear_destruir(void) {
	printf("\nINICIO DE PRUEBAS CREAR Y DESTRUIR\n");
	/* Prueba de lista creada correctamente */
	lista_t* lista = lista_crear();
	print_test("La lista se pudo crear", lista != NULL);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_largo_lista(void) {
	printf("\nINICIO DE PRUEBAS DE LARGO DE LISTA\n");
	lista_t* lista = lista_crear();
	
	/* Prueba de correcta longitud de vector */
	size_t longitud[] = {0, 10, 100};
	size_t long_vector = 3;
	
	bool correcto_largo = true;
	for (size_t i = 0; i < long_vector; i++) {
		/* Vector de prueba */
		int vector[longitud[i]];
		for(int j = 0; j < longitud[i]; j++) vector[j] = j;
		
		/* Prueba de longitud de lista al agregar elementos */ 
		for (int j = 0; j < longitud[i]; j++) {
			if (!lista_insertar_primero(lista,&vector[j])) {
				print_test("No se pudo realizar la prueba del largo de la lista debido a que no se pudo insertar elementos a la lista", false);
				lista_destruir(lista, NULL);
				return;
			}
		}
		if (lista_largo(lista) != longitud[i]) correcto_largo = false;
		
		/* Prueba de longitud de lista al eliminar elementos */
		for (int j = 0; j < longitud[i]/2; j++) lista_borrar_primero(lista);
		if (lista_largo(lista) != longitud[i]/2) correcto_largo = false;
		
		/* Vacío la lista */
		while (!lista_esta_vacia(lista)) lista_borrar_primero(lista);
	}
	
	/* Pruebo que el largo haya sido siempre el correcto */
	print_test("La lista guarda correctamente su longitud", correcto_largo == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_enlistar_alternadamente(void) {
	printf("\nINICIO DE PRUEBAS DE ENLISTAR ALTERNADAMENTE\n");
	lista_t* lista = lista_crear();
	
	/* Vector de prueba */
	size_t longitud_vector = 10;
	size_t vector[] =  {4, 5, 3, 6, 2, 7, 1, 8, 0, 9};
	
	/* Enlistado de elementos */
	for (size_t i = 0; i < longitud_vector; i++) {
		if (i%2 == 0) {
			if (!lista_insertar_primero(lista, &vector[i])) {
				print_test("No se pudo realizar la prueba de enlistar alternadamente de la lista debido a que no se pudo insertar elementos a la lista", false);
				lista_destruir(lista, NULL);
				return;
			}
		}
		else {
			if (!lista_insertar_ultimo(lista, &vector[i])) {
				print_test("No se pudo realizar la prueba de enlistar alternadamente de la lista debido a que no se pudo insertar elementos a la lista", false);
				lista_destruir(lista, NULL);
				return;
			}
		}
	}

	/* Desenlistado de elementos */
	bool desenlistado = true;
	for (size_t i = 0; i < longitud_vector; i++) {
		size_t* quitado = lista_borrar_primero(lista);
		if (*quitado != i) desenlistado = false;
	}
	
	/* Verifico que los elementos hayan sido enlistados correctamente */
	print_test("Al enlistar alternadamente entre el principio y el final, los elementos se desenlistan en el orden correcto", desenlistado == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_volumen_primero(size_t cant_elementos) {
	printf("\nINICIO DE PRUEBAS DE VOLUMEN (INSERTAR POR PRIMERO)\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int elementos [cant_elementos];
	for(int j = 0; j < cant_elementos; j++) elementos[j] = j;
	
	/* Prueba de enlistado a través del principio */
	bool enlistado_primero = true;
	bool enlistado_ultimo = true;
	int i = 0;
	for (; i < cant_elementos; i++) {
		if (!lista_insertar_primero(lista, &elementos[i])) {
			print_test("No se pudo realizar la prueba de volumen enlistando desde el principio debido a que no se pudo insertar elementos a la lista", false);
			lista_destruir(lista, NULL);
			return;
		}
		if (lista_ver_primero(lista) != &elementos[i]) enlistado_primero = false;
		if (lista_ver_ultimo(lista) != &elementos[0]) enlistado_ultimo = false;
	}

	/* Desenlistado de elementos */
	bool desenlistado = true;
	for (; i > 0; i--) {
		int* quitado = lista_borrar_primero(lista);
		if (quitado != &elementos[i-1]) desenlistado = false;
	}

	/* Prueba de lista vacía al desenlistar todos los elementos */
	bool vacio = true;
	if(!lista_esta_vacia(lista)) vacio = false;
	
	/* Pruebas a realizar en la lista enlistada desde el principio */
	print_test("Al enlistar desde el principio, el primer elemento de la lista siempre es el último elemento enlistado", enlistado_primero == true);
	print_test("Al enlistar desde el principio, el último elemento de la lista siempre es el primer elemento enlistado", enlistado_ultimo == true);
	print_test("Al enlistar desde el principio, se cumple el orden al desenlistar", desenlistado == true);
	print_test("Al enlistar desde el principio, los elementos se desenlistan hasta llegar al vacio", vacio == true);
	
	/* Destruyo vector y lista */
	lista_destruir(lista, NULL);
}

static void prueba_volumen_ultimo(size_t cant_elementos) {
	printf("\nINICIO DE PRUEBAS DE VOLUMEN (INSERTAR POR ULTIMO)\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int elementos [cant_elementos];
	for(int j = 0; j < cant_elementos; j++) elementos[j] = j;
	
	/* Prueba de enlistado a través del final */
	bool enlistado_primero = true;
	bool enlistado_ultimo = true;
	int i = 0;
	for (; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista, &elementos[i])) {
			print_test("No se pudo realizar la prueba de volumen enlistando desde el final debido a que no se pudo insertar elementos a la lista", false);
			lista_destruir(lista, NULL);
			return;
		}
		if (lista_ver_primero(lista) != &elementos[0]) enlistado_primero = false;
		if (lista_ver_ultimo(lista) != &elementos[i]) enlistado_ultimo = false;
	}

	/* Desenlistado de elementos */
	bool desenlistado = true;
	for (; i > 0; i--) {
		int* quitado = lista_borrar_primero(lista);
		if (quitado != &elementos[cant_elementos - i]) desenlistado = false;
	}

	/* Prueba de lista vacía al desenlistar todos los elementos */
	bool vacio = true;
	if(!lista_esta_vacia(lista)) vacio = false;
	
	/* Pruebas a realizar en la lista enlistada desde el principio */
	print_test("Al enlistar desde el final, el primer elemento de la lista siempre es el último elemento enlistado", enlistado_primero == true);
	print_test("Al enlistar desde el final, el último elemento de la lista siempre es el primer elemento enlistado", enlistado_ultimo == true);
	print_test("Al enlistar desde el final, se cumple el orden al desenlistar", desenlistado == true);
	print_test("Al enlistar desde el final, los elementos se desenlistan hasta llegar al vacio", vacio == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_enlistar_null(void) {
	printf("\nINICIO DE PRUEBAS DE ENLISTAR NULL\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int cant_elementos = 10;
	int elementos [cant_elementos];
	for(int j = 0; j < 10; j++) elementos[j] = j;
	
	/* Prueba de enlistado a través del final */
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista, &elementos[i])) {
			print_test("No se pudo realizar la prueba de enlistar un elemento NULL debido a que no se pudo insertar elementos a la lista", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Enlisto el elemento NULL */
	if (!lista_insertar_primero(lista, NULL)) {
		print_test("No se pudo realizar la prueba de enlistar un elemento NULL debido a que no se pudo insertar correctamente", false);
		lista_destruir(lista, NULL);
		return;
	}

	/* Prueba de enlistar elemento NULL */
	print_test("El elemento NULL es enlistable", lista_ver_primero(lista) == NULL);

	/* Destruyo cola */
	lista_destruir(lista, NULL);
}

static void prueba_enlistar_vaciar_recien_creada(void) {
	printf("\nINICIO DE PRUEBAS DE VACIAR UNA LISTA RECIÉN CREADA\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos elementos */
	int cant_elementos = 100;
	int elementos [cant_elementos];
	for(int j = 0; j < cant_elementos; j++) elementos[j] = j;
	
	/* Enlistado de elementos */
	for (int i = 0; i < cant_elementos; i++) {
		if(!lista_insertar_primero(lista, &elementos[i])) {
			print_test("No se pudo realizar la prueba de enlistar y vaciar una lista debido a que no se pudo insertar correctamente", false);
			lista_destruir(lista, NULL);
			return;
		}
	}

	/* Desenlistado de elementos */
	for (int i = 0; i < cant_elementos; i++) lista_borrar_primero(lista);
	
	/* Prueba de que lista desenlistada se comporta como una lista vacía (no se ve primero ni se puede desenlistar) */
	bool desenlistado = true;
	if (lista_ver_primero(lista) != NULL || lista_borrar_primero(lista) != NULL) desenlistado = false;
	print_test("Al ser desenlistada, la lista se comporta como recién creada", desenlistado == true);
	
	/* Destruyo vector y lista */
	lista_destruir(lista, NULL);
}

static void prueba_lista_recien_creada_desenlistar_ver_primero(void) {
	printf("\nINICIO DE PRUEBAS DE DESENLISTAR UNA LISTA RECIÉN CREADA\n");
	lista_t* lista = lista_crear();

	/* Prueba de ver primero vacío y de no poder desenlistar al recién crear la lista */
	bool recien_creada = true;
	if (lista_ver_primero(lista) != NULL || lista_borrar_primero(lista) != NULL) recien_creada = false;
	print_test("La lista al recién crearse no puede verse el primero ni desencolar", recien_creada == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_lista_recien_creada_vacia(void) {
	printf("\nINICIO DE PRUEBAS DE LISTA RECIEN CREADA ESTA VACIA\n");
	lista_t* lista = lista_crear();

	/* Prueba de lista vacía al crearse */
	print_test("La lista al recién crearse está vacía", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_sin_funcion(void) {
	printf("\nINICIO DE PRUEBAS DE DESTRUIR UNA LISTA VACIA SIN FUNCION\n");
	lista_t* lista = lista_crear();

	/* Prueba de destruir lista vacía con función de destrucción */
	print_test("Se destruye una lista vacía sin utilizar una función de destruccion (revisar valgrind para ver el resultado de esta prueba)", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_con_funcion(void) {
	printf("\nINICIO DE PRUEBAS DE DESTRUIR UNA LISTA VACIA CON FUNCION\n");
	lista_t* lista = lista_crear();

	/* Prueba de destruir lista vacía con función de destrucción */
	print_test("Se destruye una lista vacía utilizando una función de destruccion (revisar valgrind para ver el resultado de esta prueba)", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, free);
}

static void prueba_enlistar_colas_destruir(void) {
	printf("\nINICIO DE PRUEBAS DE ENLISTAR Y DESTRUIR COLAS\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchas colas */
	int cant_colas = 100;
	
	/* Enlistado de colas */
	for (int i = 0; i < cant_colas; i ++) {
		if (!lista_insertar_primero(lista,cola_crear())) {
			print_test("No se pudo realizar la prueba de enlistar colas debido a que no se los pudo insertar correctamente", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Destruyo lista utilizando la función correspondiente para las colas */
	lista_destruir(lista,destruir_cola);
	print_test("Se enlistan colas y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_colas);
}

static void prueba_enlistar_pilas_destruir(void) {
	printf("\nINICIO DE PRUEBAS DE ENLISTAR Y DESTRUIR PILAS\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchas pilas */
	int cant_pilas = 100;
	
	/* Enlistado de pilas */
	for (int i = 0; i < cant_pilas; i++) {
		if (!lista_insertar_primero(lista,pila_crear())) {
			print_test("No se pudo realizar la prueba de enlistar pilas debido a que no se los pudo insertar correctamente", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Destruyo lista utilizando la función correspondiente para las pilas */
	lista_destruir(lista,destruir_pila);
	print_test("Se enlistan pilas y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_pilas);
}

static void prueba_enlistar_vectores_destruir(void) {
	printf("\nINICIO DE PRUEBAS DE ENLISTAR Y DESTRUIR VECTORES\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos vectores */
	int cant_vecs = 100;
	size_t tam_vec = 2;
	
	/* Enlistado de vectores */
	for (int i = 0; i < cant_vecs; i++) {
		if (!lista_insertar_primero(lista,vector_crear(tam_vec))) {
			print_test("No se pudo realizar la prueba de enlistar vectores debido a que no se los pudo insertar correctamente", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Destruyo lista utilizando la función correspondiente para los vectores */
	lista_destruir(lista,destruir_vector_TDA);
	print_test("Se enlistan vectores y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_vecs);
}

static void prueba_enlistar_punterosdinamicos_destruir(void) {
	printf("\nINICIO DE PRUEBAS DE ENLISTAR Y DESTRUIR PUNTEROS DINAMICOS\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con muchos punteros */
	int cant_punts = 100;
	
	/* Enlistado de punteros */
	for (int i = 0; i < cant_punts; i++) {
		void* puntero = malloc(sizeof(void*));
		if (puntero == NULL) {
			print_test("No se pudo probar el enlistado de punteros debido a que no hay memoria dinamica para un puntero", puntero != NULL);
			lista_destruir(lista,free);
			return;
		}
		if (!lista_insertar_primero(lista,puntero)) {
			print_test("No se pudo probar el enlistado de punteros debido a que no se los pudo insertar correctamente", false);
			free(puntero);
			lista_destruir(lista,free);
			return;
		}
	}
	
	/* Destruyo lista utilizando la función correspondiente para los punteros */
	lista_destruir(lista,free);
	print_test("Se enlistan punteros y luego la lista es destruida (revisar valgrind para ver el resultado de esta prueba)", cant_punts);
}

static void prueba_iterador_externo_insertar_principio(void) {
	printf("\nINICIO DE PRUEBAS DE INSERTAR UN ELEMENTO AL PRINCIPIO CON ITERADOR EXTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo inserta al principio de la lista al apenas ser creado debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Inserto apenas creo el iterador */
	int cero = 0;
	lista_iter_t* iter = lista_iter_crear(lista);
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo inserta al principio de la lista al apenas ser creado debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	if (!lista_iter_insertar(iter,(void*)&cero)) {
		print_test("No se pudo probar que el iterador externo inserta al principio de la lista al apenas ser creado debido a que no se pudo insertar correctamente", false);
		lista_iter_destruir(iter);
		lista_destruir(lista, NULL);
	}
	
	/* Verifico que el elemento se haya insertado al final */
	print_test("El iterador externo inserta al principio de la lista al apenas ser creado", *(int*)lista_iter_ver_actual(iter) == *(int*)lista_ver_primero(lista));
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_insertar_medio(void) {
	printf("\nINICIO DE PRUEBAS DE INSERTAR UN ELEMENTO EN EL MEDIO CON ITERADOR EXTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if(!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo inserta al medio de la lista debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Avanzo hasta el medio de la lista con el iterador */
	lista_iter_t* iter = lista_iter_crear(lista);
	size_t lista_mitad = lista_largo(lista)/2;
	size_t cont_avance = 0;
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo inserta al medio de la lista debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	while (cont_avance != lista_mitad) {
		lista_iter_avanzar(iter);
		cont_avance++;
	}
	
	/* Inserto un elemento */
	int primero_inicial = *(int*)lista_ver_primero(lista);
	int medio_inicial = *(int*)lista_iter_ver_actual(iter);
	int ultimo_inicial = *(int*)lista_ver_ultimo(lista);
	int once = 11;
	if (!lista_iter_insertar(iter,(void*)&once)) {
		print_test("No se pudo probar que el iterador externo inserta al medio de la lista debido a que no se pudo insertar correctamente", false);
		lista_destruir(lista, NULL);
	}
	
	/* Verifico que el elemento se insertó al medio de la lista */
	lista_iter_t* iter_nuevo = lista_iter_crear(lista);
	int i = 0;
	while (i < lista_mitad) {
		lista_iter_avanzar(iter_nuevo);
		i++;
	}

	print_test("El iterador externo se encuentra en el elemento insertado", *(int*)lista_iter_ver_actual(iter_nuevo) == once);
	lista_iter_avanzar(iter_nuevo);
	print_test("El elemento que antes se encontraba en el medio ahora es el siguiente al insertado", *(int*)lista_iter_ver_actual(iter_nuevo) == medio_inicial);
	print_test("El iterador externo mantiene como primer elemento al que se tenía previo a insertar un elemento en el medio", *(int*)lista_ver_primero(lista) == primero_inicial);
	print_test("El iterador externo mantiene como último elemento al que se tenía previo a insertar un elemento en el medio", *(int*)lista_ver_ultimo(lista) == ultimo_inicial);
		
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter);
	lista_iter_destruir(iter_nuevo);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_insertar_final(void) {
	printf("\nINICIO DE PRUEBAS DE INSERTAR UN ELEMENTO AL FINAL CON ITERADOR EXTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if(!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo inserta al final de la lista debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Avanzo hasta el final de la lista con el iterador */
	lista_iter_t* iter = lista_iter_crear(lista);
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo inserta al final de la lista debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	while(!lista_iter_al_final(iter)) lista_iter_avanzar(iter);
	
	/* Inserto un elemento */
	int once = 11;
	if (!lista_iter_insertar(iter,(void*)&once)) {
		print_test("No se pudo probar que el iterador externo inserta al final de la lista debido a que no se pudo insertar correctamente", false);
		lista_destruir(lista, NULL);
	}
	
	/* Verifico que el elemento se insertó al final de la lista */
	print_test("El iterador externo inserta al final de la lista si el mismo se encuentra allí", *(int*)lista_iter_ver_actual(iter) == *(int*)lista_ver_ultimo(lista));
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_borrar_principio(void) {
	printf("\nINICIO DE PRUEBAS DE BORRAR EL PRIMER ELEMENTO CON ITERADOR EXTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int primer_elemento = elementos[0];
	int segundo_elemento = elementos[1];
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo borra al principio de la lista al apenas ser creado debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Borro apenas creo el iterador */
	lista_iter_t* iter = lista_iter_crear(lista);
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo borra al principio de la lista al apenas ser creado debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	
	/* Verifico que el elemento borrado sea el primero y que el primero de la lista haya cambiado correctamente */
	print_test("El iterador externo borra al principio de la lista al apenas ser creado", *(int*)lista_iter_borrar(iter) == primer_elemento && *(int*)lista_ver_primero(lista) == segundo_elemento);
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_borrar_medio(void) {
	printf("\nINICIO DE PRUEBAS DE BORRAR EL ELEMENTO DEL MEDIO CON ITERADOR EXTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if(!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo borra el elemento del medio de la lista debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}

	/* Avanzo hasta el medio de la lista con el iterador */
	lista_iter_t* iter = lista_iter_crear(lista);
	size_t lista_mitad = lista_largo(lista)/2;
	size_t cont_avance = 0;
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo borra el elemento del medio de la lista debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	while (cont_avance != lista_mitad) {
		lista_iter_avanzar(iter);
		cont_avance++;
	}
	
	/* Borro un elemento */
	int medio_borrar = *(int*)lista_iter_ver_actual(iter);
	int primero_inicial = *(int*)lista_ver_primero(lista);
	int ultimo_inicial = *(int*)lista_ver_ultimo(lista);
	
	/* Verifico que el elemento del medio se borre de la lista */
	print_test("El iterador externo borró el elemento del medio", *(int*)lista_iter_borrar(iter) == medio_borrar);
	
	/* Verifico que la lista cambió como se esperaba tras borrar el elemento en el medio */
	lista_iter_t* iter_nuevo = lista_iter_crear(lista);
	int i = 0;
	while (i < lista_mitad) {
		lista_iter_avanzar(iter_nuevo);
		i++;
	}
	
	print_test("El elemento que se encuentra en el medio ya no es el que se quiso borrar", *(int*)lista_iter_ver_actual(iter_nuevo) != medio_borrar);
	print_test("El iterador externo mantiene como primer elemento al que se tenía previo a borrar el elemento del medio", *(int*)lista_ver_primero(lista) == primero_inicial);
	print_test("El iterador externo mantiene como último elemento al que se tenía previo a borrar el elemento del medio", *(int*)lista_ver_ultimo(lista) == ultimo_inicial);
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter);
	lista_iter_destruir(iter_nuevo);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_borrar_final(void) {
	printf("\nINICIO DE PRUEBAS DE BORRAR EL ELEMENTO FINAL CON ITERADOR EXTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	int anteultimo_elemento = elementos[cant_elementos-2];
	int ultimo_elemento = elementos[cant_elementos-1];
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo borra correctamente al final de la lista debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Avanzo hasta el ultimo elemento de la lista con el iterador */
	lista_iter_t* iter = lista_iter_crear(lista);
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo borra correctamente al final de la lista debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	while(lista_iter_ver_actual(iter) != lista_ver_ultimo(lista)) lista_iter_avanzar(iter);
	
	/* Verifico que el elemento borrado sea el último y que se cambió correctamente el final de la lista */
	print_test("El iterador externo borra correctamente el último elemento de la lista", *(int*)lista_iter_borrar(iter) == ultimo_elemento && *(int*)lista_ver_ultimo(lista) == anteultimo_elemento);
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_interno_vacio(void) {
	printf("\nINICIO DE PRUEBAS DE LISTA VACIA CON ITERADOR INTERNO\n");
	lista_t* lista = lista_crear();
	
	/* Pruebas con el iterador interno vacío*/
	int primero = 0;
	lista_iterar(lista, obtener_primer_elemento_iter_interno, &primero);
	print_test("El iterador interno no puede obtener el primer elemento de la lista si está vacía", primero == 0);
	
	int ultimo = 0;
	lista_iterar(lista, obtener_ultimo_elemento_iter_interno, &ultimo);
	print_test("El iterador interno no puede obtener el ultimo elemento de la lista si está vacía", ultimo == 0);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_interno_operaciones_sin_corte(void) {
	printf("\nINICIO DE PRUEBAS DE OPERACIONES CON ITERADOR INTERNO SIN CONDICION DE CORTE\n");
	lista_t* lista = lista_crear();
	
	/* Inserto elementos a la lista a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("Fallaron las pruebas del interador interno debido a que no se pudo insertar correctamente los elementos con el iterador externo", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Pruebo el iterador interno */
	int elementos_obtenidos[cant_elementos+1];
	elementos_obtenidos[cant_elementos] = 0; /* contador */
	bool ok_obtenidos = true;
	lista_iterar(lista, obtener_elementos_iter_interno, &elementos_obtenidos);
	for (int i = 0; i < cant_elementos; i++) {
		ok_obtenidos &= (elementos[i] == elementos_obtenidos[i]);
		if (!ok_obtenidos) {
			break;
		}
	}
	print_test("Se pudieron obtener todos los elementos de la lista utilizando el iterador interno", ok_obtenidos);
	
	int ultimo = 0;
	lista_iterar(lista, obtener_ultimo_elemento_iter_interno, &ultimo);
	print_test("El iterador interno obtiene el ultimo elemento de la lista", ultimo == elementos[cant_elementos-1]);
	
	int suma_todos = 0;
	lista_iterar(lista, sumar_elementos_iter_interno, &suma_todos);
	print_test("Se pudieron sumar los valores de todos los elementos utilizando el iterador interno", suma_todos == 55);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_interno_operaciones_con_corte(void) {
	printf("\nINICIO DE PRUEBAS DE OPERACIONES CON ITERADOR INTERNO CON CONDICION DE CORTE\n");
	lista_t* lista = lista_crear();
	
	/* Inserto elementos a la lista a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("Fallaron las pruebas del interador interno debido a que no se pudo insertar correctamente los elementos con el iterador externo", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Pruebo el iterador interno */
	int primero = 0;
	lista_iterar(lista, obtener_primer_elemento_iter_interno, &primero);
	print_test("El iterador interno obtiene el primer elemento de la lista", primero == elementos[0]);
	
	int suma_cinco[] = {5,0,0}; /* {cantidad de elementos de la lista a sumar, contador, acumulador} */
	lista_iterar(lista, sumar_primeros_x_elementos_iter_interno, &suma_cinco);
	print_test("Se pudieron sumar los valores de los primeros cinco elementos utilizando el iterador interno", suma_cinco[2] == 15);
	
	int factorial_cinco[] = {5,0,1}; /* {factorial pedido, contador, acumulador} */
	lista_iterar(lista, factorial_primeros_x_elementos_iter_interno, &factorial_cinco);
	print_test("Se pudo calcular el factorial de 5 usando los valores de los primeros cinco elementos mediante el iterador interno", factorial_cinco[2] == 120);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante() {
	/* Ejecuta todas las pruebas. */
	printf("INICIO DE PRUEBAS UNITARIAS\n");
	prueba_crear_destruir();
	prueba_largo_lista();
	prueba_enlistar_alternadamente();
	prueba_volumen_primero(10000);
	prueba_volumen_ultimo(10000);
	prueba_enlistar_null();
	prueba_enlistar_vaciar_recien_creada();
	prueba_lista_recien_creada_desenlistar_ver_primero();
	prueba_lista_recien_creada_vacia();
	prueba_destruir_lista_vacia_sin_funcion();
	prueba_destruir_lista_vacia_con_funcion();
	prueba_enlistar_colas_destruir();
	prueba_enlistar_pilas_destruir();
	prueba_enlistar_vectores_destruir();
	prueba_enlistar_punterosdinamicos_destruir();
	prueba_iterador_externo_insertar_principio();
	prueba_iterador_externo_insertar_medio();
	prueba_iterador_externo_insertar_final();
	prueba_iterador_externo_borrar_principio();
	prueba_iterador_externo_borrar_medio();
	prueba_iterador_externo_borrar_final();
	prueba_iterador_interno_vacio();
	prueba_iterador_interno_operaciones_sin_corte();
	prueba_iterador_interno_operaciones_con_corte();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
	pruebas_lista_estudiante();
	return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
