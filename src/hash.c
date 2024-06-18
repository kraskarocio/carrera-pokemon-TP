#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACIDAD_MINIMA 3
#define FACTOR_CARGA_MAXIMA 0.7

typedef struct {
	char *clave;
	void *valor;
	bool ocupado;
} par_t;

typedef struct {
	size_t capacidad;
	size_t cantidad;
	par_t *elementos;
} hash_t;

char *copiar_clave(const char *string_a_copiar)
{
	if (!string_a_copiar)
		return NULL;

	char *string_copiado = malloc(strlen(string_a_copiar) + 1);
	if (!string_copiado)
		return NULL;

	strcpy(string_copiado, string_a_copiar);
	return string_copiado;
}

size_t hash_funcion_djb2(const char *clave)
{
	size_t hash = 5381;
	int c;
	while ((c = (int)*clave++))
		hash = ((hash << 5) + hash) + (size_t)c;

	return hash;
}

float calcular_factor_carga(hash_t *hash)
{
	return (float)(hash->cantidad) / (float)(hash->capacidad);
}

void rehash(hash_t *hash)
{
	size_t nueva_capacidad = hash->capacidad * 2;
	par_t *nuevos_elementos = calloc(nueva_capacidad, sizeof(par_t));
	if (!nuevos_elementos)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->elementos[i].ocupado) {
			size_t nuevo_indice =
				hash_funcion_djb2(hash->elementos[i].clave) %
				nueva_capacidad;

			while (nuevos_elementos[nuevo_indice].ocupado)
				nuevo_indice =
					(nuevo_indice + 1) % nueva_capacidad;

			nuevos_elementos[nuevo_indice].clave =
				copiar_clave(hash->elementos[i].clave);
			if (!nuevos_elementos[nuevo_indice].clave) {
				for (size_t j = 0; j < i; j++) {
					free(nuevos_elementos[j].clave);
				}
				free(nuevos_elementos);
				return;
			}
			nuevos_elementos[nuevo_indice].valor =
				hash->elementos[i].valor;
			nuevos_elementos[nuevo_indice].ocupado = true;
		}
	}
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->elementos[i].ocupado) {
			free(hash->elementos[i].clave);
		}
	}
	free(hash->elementos);
	hash->elementos = nuevos_elementos;
	hash->capacidad = nueva_capacidad;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;

	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->elementos = calloc(capacidad, sizeof(par_t));
	if (!hash->elementos) {
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;
	if (anterior)
		*anterior = NULL;

	if (calcular_factor_carga(hash) > FACTOR_CARGA_MAXIMA)
		rehash(hash);

	size_t indice = hash_funcion_djb2(clave) % hash->capacidad;
	while (hash->elementos[indice].ocupado &&
	       strcmp(hash->elementos[indice].clave, clave) != 0)
		indice = (indice + 1) % hash->capacidad;

	if (hash->elementos[indice].ocupado) {
		if (anterior)
			*anterior = hash->elementos[indice].valor;
		hash->elementos[indice].valor = elemento;
		free(hash->elementos[indice].clave);
		hash->elementos[indice].clave = copiar_clave(clave);
		if (!hash->elementos[indice].clave)
			return NULL;
	} else {
		hash->elementos[indice].clave = copiar_clave(clave);
		if (!hash->elementos[indice].clave)
			return NULL;
		hash->elementos[indice].valor = elemento;
		hash->elementos[indice].ocupado = true;
		hash->cantidad++;
	}

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t indice = hash_funcion_djb2(clave) % hash->capacidad;
	while (hash->elementos[indice].ocupado) {
		if (strcmp(hash->elementos[indice].clave, clave) == 0) {
			void *valor_eliminado = hash->elementos[indice].valor;
			free(hash->elementos[indice].clave);
			hash->elementos[indice].clave = NULL;
			hash->elementos[indice].valor = NULL;
			hash->elementos[indice].ocupado = false;
			hash->cantidad--;
			size_t siguiente_indice =
				(indice + 1) % hash->capacidad;
			while (hash->elementos[siguiente_indice].ocupado) {
				par_t elemento_movido =
					hash->elementos[siguiente_indice];
				hash->elementos[siguiente_indice].ocupado =
					false;
				hash->cantidad--;
				hash_insertar(hash, elemento_movido.clave,
					      elemento_movido.valor, NULL);
				free(elemento_movido.clave);

				siguiente_indice = (siguiente_indice + 1) %
						   hash->capacidad;
			}
			return valor_eliminado;
		}

		indice = (indice + 1) % hash->capacidad;
	}

	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t indice = hash_funcion_djb2(clave) % hash->capacidad;

	while (hash->elementos[indice].ocupado) {
		if (strcmp(hash->elementos[indice].clave, clave) == 0)
			return hash->elementos[indice].valor;
		indice = (indice + 1) % hash->capacidad;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	size_t indice = hash_funcion_djb2(clave) % hash->capacidad;

	while (hash->elementos[indice].ocupado) {
		if (strcmp(hash->elementos[indice].clave, clave) == 0)
			return true;
		indice = (indice + 1) % hash->capacidad;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}
void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->elementos[i].ocupado) {
			free(hash->elementos[i].clave);
			if (destructor)
				destructor(hash->elementos[i].valor);
		}
	}

	free(hash->elementos);
	free(hash);
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
	return;
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *, void *, void *), void *aux)
{
	if (!hash || !f)
		return 0;

	size_t contador_elementos_hash = 0;

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->elementos[i].ocupado) {
			contador_elementos_hash++;
			if (!f(hash->elementos[i].clave,
			       hash->elementos[i].valor, aux))
				break;
		}
	}

	return contador_elementos_hash;
}
