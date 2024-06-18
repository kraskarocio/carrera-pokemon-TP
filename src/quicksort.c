#include "quicksort.h"

void swap(char **a, char **b)
{
	char *aux = *a;
	*a = *b;
	*b = aux;
}

int particion(char **elementos, int inicio, int fin)
{
	char *pivot = elementos[fin];
	int pivot_index = inicio - 1;

	for (int j = inicio; j < fin; j++) {
		if (strcmp(elementos[j], pivot) < 0) {
			pivot_index++;
			swap(&elementos[pivot_index], &elementos[j]);
		}
	}
	swap(&elementos[pivot_index + 1], &elementos[fin]);
	return pivot_index + 1;
}

void quicksort_recursivo(char **elementos, int inicio, int fin)
{
	if (inicio < fin) {
		int pivot_indice = particion(elementos, inicio, fin);

		quicksort_recursivo(elementos, inicio, pivot_indice - 1);
		quicksort_recursivo(elementos, pivot_indice + 1, fin);
	}
}

void quicksort_alfabetico(char **elementos, size_t cant)
{
	if (elementos == NULL || cant == 0)
		return;
	quicksort_recursivo(elementos, 0, (int)cant - 1);
}