#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	size_t tamaño;
	nodo_t *nodo_inicio;
	nodo_t *nodo_fin;
};

struct lista_iterador {
	nodo_t *corriente;
	lista_t *lista;
};

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}
	nodo_t *nodo_nuevo = calloc(1, sizeof(nodo_t));
	if (!nodo_nuevo) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;
	if (!lista->nodo_inicio) {
		lista->nodo_inicio = nodo_nuevo;
	} else {
		lista->nodo_fin->siguiente = nodo_nuevo;
	}
	lista->nodo_fin = nodo_nuevo;
	lista->tamaño++;
	return lista;
}

//Función recursiva que busca el nodo que está en X poscición
nodo_t *devuelve_pt_a_nodo(nodo_t *nodo, size_t posicion)
{
	if (!nodo || posicion == 0) {
		return nodo;
	}
	return devuelve_pt_a_nodo(nodo->siguiente, posicion - 1);
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	nodo_t *nodo_nuevo = calloc(1, sizeof(nodo_t));
	if (!nodo_nuevo) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;
	if (posicion >= lista->tamaño) {
		free(nodo_nuevo);
		return lista_insertar(lista, elemento);
	} else if (posicion == (size_t)0) {
		nodo_nuevo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo_nuevo;
	} else {
		nodo_t *nodo_previo =
			devuelve_pt_a_nodo(lista->nodo_inicio, posicion - 1);
		nodo_nuevo->siguiente = nodo_previo->siguiente;
		nodo_previo->siguiente = nodo_nuevo;
	}

	lista->tamaño++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista->tamaño == 0)
		return NULL;
	nodo_t *nodo_a_quitar = lista->nodo_fin->elemento;
	if (lista->tamaño == 1) {
		free(lista->nodo_fin);
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
	} else if (lista->tamaño == 2) {
		free(lista->nodo_fin);
		lista->nodo_fin = lista->nodo_inicio;
	} else {
		nodo_t *anterior_nodo = devuelve_pt_a_nodo(lista->nodo_inicio,
							   lista->tamaño - 2);
		free(lista->nodo_fin);
		lista->nodo_fin = anterior_nodo;
	}
	lista->tamaño--;
	return (nodo_a_quitar);
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista->tamaño == 0) {
		return NULL;
	}
	if (posicion == 0) {
		nodo_t *elemento_a_quitar = lista->nodo_inicio->elemento;
		nodo_t *nodo_a_quitar = lista->nodo_inicio;
		lista->nodo_inicio = nodo_a_quitar->siguiente;
		free(nodo_a_quitar);
		lista->tamaño--;
		return elemento_a_quitar;
	} else if (posicion >= lista->tamaño - 1) {
		return lista_quitar(lista);
	}
	nodo_t *nodo_precesor =
		devuelve_pt_a_nodo(lista->nodo_inicio, posicion - 1);
	nodo_t *nodo_a_quitar = nodo_precesor->siguiente;
	nodo_t *elemento_a_quitar = nodo_a_quitar->elemento;
	nodo_precesor->siguiente = nodo_a_quitar->siguiente;

	free(nodo_a_quitar);
	lista->tamaño--;
	return elemento_a_quitar;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista) || (posicion >= lista->tamaño)) {
		return NULL;
	}
	nodo_t *nodo_actual = devuelve_pt_a_nodo(lista->nodo_inicio, posicion);
	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (lista_vacia(lista) == true || comparador == NULL)
		return NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;

	while (nodo_actual != NULL) {
		if (comparador(nodo_actual->elemento, contexto) == 0) {
			return nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;
	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista)
		return true;
	return (!(lista->nodo_inicio) || !(lista->nodo_fin) ||
		!(lista->tamaño));
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return 0;
	return lista->tamaño;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;
	while (lista->tamaño > 0) {
		lista_quitar_de_posicion(lista, 0);
	}
	free(lista);
}
void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista || !funcion) {
		return;
	}
	while (!lista_vacia(lista)) {
		void *dato = lista_quitar(lista);
		if (funcion) {
			funcion(dato);
		}
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;

	lista_iterador_t *iterador = calloc(1, sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	return (iterador && iterador->corriente != NULL);
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente) {
		return false;
	}
	if (iterador->corriente == iterador->lista->nodo_fin) {
		iterador->corriente = NULL;
		return false;
	}
	iterador->corriente = iterador->corriente->siguiente;
	return (iterador->corriente != NULL);
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente) {
		return NULL;
	}
	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion)
		return 0;

	size_t contador = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	bool seguir = true;

	while (nodo_actual && seguir) {
		seguir = funcion(nodo_actual->elemento, contexto);
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}

	return contador;
}