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

/* *****************************************************************
 *               	FUNCIONES DE PRUEBA
 * *****************************************************************/

static void prueba_crear_destruir(void) {
	/* Prueba de lista creada correctamente */
	lista_t* lista = lista_crear();
	print_test("La lista se pudo crear", lista != NULL);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_largo_lista(void) {
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
	lista_t* lista = lista_crear();

	/* Prueba de ver primero vacío y de no poder desenlistar al recién crear la lista */
	bool recien_creada = true;
	if (lista_ver_primero(lista) != NULL || lista_borrar_primero(lista) != NULL) recien_creada = false;
	print_test("La lista al recién crearse no puede verse el primero ni desencolar", recien_creada == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_lista_recien_creada_vacia(void) {
	lista_t* lista = lista_crear();

	/* Prueba de lista vacía al crearse */
	print_test("La cola al recién crearse está vacía", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_sin_funcion(void) {
	lista_t* lista = lista_crear();

	/* Prueba de destruir lista vacía con función de destrucción */
	print_test("Se destruye una lista vacía sin utilizar una función de destruccion (revisar valgrind para ver el resultado de esta prueba)", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_con_funcion(void) {
	lista_t* lista = lista_crear();

	/* Prueba de destruir lista vacía con función de destrucción */
	print_test("Se destruye una lista vacía utilizando una función de destruccion (revisar valgrind para ver el resultado de esta prueba)", lista_esta_vacia(lista) == true);
	
	/* Destruyo lista */
	lista_destruir(lista, free);
}

static void prueba_enlistar_colas_destruir(void) {
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

static void prueba_iterador_interno_sin_corte(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	int suma_elementos = 0;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo realizar la prueba del iterador interno sin condición de corte debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
		suma_elementos += elementos[i];
	}
	
	/* Sumo todos los elementos de la lista con el iterador interno */
	int suma_iterador = 0;
	lista_iterar(lista, sumar_elementos_de_lista, &suma_iterador);
	
	/* Verifico que la suma sea la correcta */
	print_test("Por medio del iterador interno y sin condición de corte se recorre correctamente toda la lista", suma_elementos == suma_iterador);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_interno_con_corte(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo realizar la prueba del iterador interno con condición de corte debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Recorro la lista hasta encontrar un elemento mayor que 5 */
	int elemento_final = 0;
	lista_iterar(lista, verificar_mayor_que_5, (void*)&elemento_final);
	
	/* Verifico que el elemento de salida del iterador sea mayor que 5 */
	print_test("Por medio del iterador interno y con condición de corte se recorre correctamente toda la lista", elemento_final > 5);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_interno_sin_extra(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo realizar la prueba del iterador interno sin dato extra debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Recorro la lista duplicando los elementos de la lista */
	lista_iterar(lista, duplicar_elementos_de_lista, NULL);
	
	/* Verifico que cada elemento de la lista se haya duplicado */
	bool duplicado = true;
	for (int i = 0; i < cant_elementos; i++) if (*(int*)lista_borrar_primero(lista) != 2*(i+1)) duplicado = false;
	print_test("Por medio del iterador interno y sin un dato extra se recorre correctamente toda la lista", duplicado == true);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_insertar_principio(void) {
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

static void prueba_iterador_externo_insertar_final(void) {
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

static void prueba_iterador_externo_insertar_medio(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Avanzo hasta alguna posición intermedia de la lista con el iterador */
	int pos_intermedia = cant_elementos/2;
	lista_iter_t* iter = lista_iter_crear(lista);
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	for(int i = 0; i < pos_intermedia; i++) lista_iter_avanzar(iter);
	
	/* Inserto un elemento */
	int once = 11;
	if (!lista_iter_insertar(iter,(void*)&once)) {
		print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo insertar correctamente", false);
		lista_destruir(lista, NULL);
	}
	
	/* Destruyo primer iterador de lista */
	lista_iter_destruir(iter);
	
	/* Creo segundo iterador para buscar el elemento correspondiente */
	lista_iter_t* iter2 = lista_iter_crear(lista);
	if (iter2 == NULL) {
		print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo crear el iterador", iter2 != NULL);
		lista_destruir(lista,free);
		return;
	}
	for(int i = 0; i < pos_intermedia; i++) lista_iter_avanzar(iter2);
	
	/* Verifico que el elemento donde se encuentra el iterador sea el insertado */
	print_test("El iterador externo inserta correctamente en el medio de la lista", *(int*)lista_iter_ver_actual(iter2) == once);
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter2);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}

static void prueba_iterador_externo_borrar_principio(void) {
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

static void prueba_iterador_externo_borrar_final(void) {
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

static void prueba_iterador_externo_borrar_medio(void) {
	lista_t* lista = lista_crear();
	
	/* Enlistado con elementos a través del último */
	int elementos[] = {1,2,3,4,5,6,7,8,9,10};
	int cant_elementos = 10;
	for (int i = 0; i < cant_elementos; i++) {
		if (!lista_insertar_ultimo(lista,&elementos[i])) {
			print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo insertar correctamente los elementos", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	
	/* Avanzo hasta alguna posición intermedia de la lista con el iterador */
	int pos_intermedia = cant_elementos/2;
	lista_iter_t* iter = lista_iter_crear(lista);
	if (iter == NULL) {
		print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo crear el iterador", iter != NULL);
		lista_destruir(lista,free);
		return;
	}
	for(int i = 0; i < pos_intermedia; i++) lista_iter_avanzar(iter);
	
	/* Borro un elemento */
	int elemento_borrado = *(int*)lista_iter_borrar(iter);
	
	/* Destruyo primer iterador de lista */
	lista_iter_destruir(iter);
	
	/* Creo segundo iterador para buscar el elemento correspondiente */
	lista_iter_t* iter2 = lista_iter_crear(lista);
	if (iter2 == NULL) {
		print_test("No se pudo probar que el iterador externo inserta correctamente en el medio de la lista debido a que no se pudo crear el iterador", iter2 != NULL);
		lista_destruir(lista,free);
		return;
	}
	for(int i = 0; i < pos_intermedia; i++) lista_iter_avanzar(iter2);
	
	/* Verifico que el elemento donde se encuentra el iterador no sea el que se borró */
	print_test("El iterador externo borra correctamente en el medio de la lista", *(int*)lista_iter_ver_actual(iter2) != elemento_borrado);
	
	/* Destruyo iterador de lista */
	lista_iter_destruir(iter2);
	
	/* Destruyo lista */
	lista_destruir(lista, NULL);
}


void pruebas_lista_estudiante() {
    /* Ejecuta todas las pruebas. */
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
    prueba_iterador_interno_sin_corte();
    prueba_iterador_interno_con_corte();
    prueba_iterador_interno_sin_extra();
    prueba_iterador_externo_insertar_principio();
    prueba_iterador_externo_insertar_final();
    prueba_iterador_externo_insertar_medio();
    prueba_iterador_externo_borrar_principio();
    prueba_iterador_externo_borrar_final();
    prueba_iterador_externo_borrar_medio();
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
