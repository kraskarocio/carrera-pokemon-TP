#include "tp.h"
#include "lista.h"
#include "hash.h"
#include "quicksort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 100
#define BUFFER_SIZE_MINI 6
struct jugador {
	struct pokemon_info *pokemon;
	lista_t *pista;
};

/*******************************************************************
 *               Funciones y estructuras auxiliares
 ******************************************************************/

char *copiar_nombre(const char *string_a_copiar)
{
	if (!string_a_copiar)
		return NULL;

	char *string_copiado = malloc(strlen(string_a_copiar) + 1);
	if (!string_copiado)
		return NULL;

	strcpy(string_copiado, string_a_copiar);
	return string_copiado;
}

typedef struct {
	char **nombres;
	size_t cantidad;
	size_t capacidad;
	TP *tp;
} nombres_t;

bool agregar_clave(const char *clave, void *valor, void *aux)
{
	nombres_t *nombres = (nombres_t *)aux;
	const TP *tp = nombres->tp;
	if ((tp_pokemon_seleccionado((TP *)tp, JUGADOR_1) &&
	     strcmp(tp_pokemon_seleccionado((TP *)tp, JUGADOR_1)->nombre,
		    clave) == 0) ||
	    (tp_pokemon_seleccionado((TP *)tp, JUGADOR_2) &&
	     strcmp(tp_pokemon_seleccionado((TP *)tp, JUGADOR_2)->nombre,
		    clave) == 0)) {
		return true;
	}

	if (nombres->cantidad == nombres->capacidad) {
		size_t nueva_capacidad = nombres->capacidad * 2;
		char **nuevos_nombres = realloc(
			nombres->nombres, nueva_capacidad * sizeof(char *));
		if (!nuevos_nombres) {
			return false;
		}
		nombres->nombres = nuevos_nombres;
		nombres->capacidad = nueva_capacidad;
	}

	nombres->nombres[nombres->cantidad] = copiar_nombre(clave);
	if (!nombres->nombres[nombres->cantidad]) {
		return false;
	}
	nombres->cantidad++;
	return true;
}

int comparar(const void *a, const void *b)
{
	return strcmp(*(const char **)a, *(const char **)b);
}

typedef struct {
	char *obstaculos_pista;
	size_t posicion;
} obstaculos_pista_aux_t;

bool agregar_obstaculo_a_string(void *obstaculo, void *contexto)
{
	obstaculos_pista_aux_t *aux = (obstaculos_pista_aux_t *)contexto;
	char letra_obstaculo = '\0';
	switch ((enum TP_OBSTACULO)obstaculo) {
	case OBSTACULO_FUERZA:
		letra_obstaculo = 'F';
		break;
	case OBSTACULO_DESTREZA:
		letra_obstaculo = 'D';
		break;
	case OBSTACULO_INTELIGENCIA:
		letra_obstaculo = 'I';
		break;
	default:
		return false;
	}
	aux->obstaculos_pista[aux->posicion] = letra_obstaculo;
	aux->posicion++;
	return true;
}

bool liberar(const char *clave, void *valor, void *aux)
{
	struct pokemon_info *pokemon = (struct pokemon_info *)valor;
	free(pokemon->nombre);
	free(pokemon);
	return true;
}

void liberar_jugador(struct jugador *jugador)
{
	if (jugador) {
		if (jugador->pokemon) {
			free(jugador->pokemon->nombre);
			free(jugador->pokemon);
		}
		if (jugador->pista) {
			lista_destruir(jugador->pista);
		}
		free(jugador);
	}
}

struct jugador *asignar_jugador(TP *tp, struct jugador *jugador_actual,
				enum TP_JUGADOR jugador_enum)
{
	if (jugador_enum == JUGADOR_1) {
		jugador_actual = tp->jugador1;
	} else {
		jugador_actual = tp->jugador2;
	}
	return jugador_actual;
}

/*******************************************************************
 *                      Funciones del TP
 ******************************************************************/

TP *tp_crear(const char *nombre_archivo)
{
	if (!nombre_archivo)
		return NULL;

	FILE *file = fopen(nombre_archivo, "r");
	if (!file)
		return NULL;

	TP *tp = malloc(sizeof(TP));
	if (!tp) {
		fclose(file);
		return NULL;
	}

	tp->hash_pokemones = hash_crear(10);
	if (!tp->hash_pokemones) {
		fclose(file);
		free(tp);
		return NULL;
	}
	tp->jugador1 = malloc(sizeof(struct jugador));
	if (!tp->jugador1) {
		fclose(file);
		hash_destruir(tp->hash_pokemones);
		free(tp);
		return NULL;
	}
	tp->jugador1->pista = NULL;
	tp->jugador1->pokemon = NULL;

	tp->jugador2 = malloc(sizeof(struct jugador));
	if (!tp->jugador2) {
		fclose(file);
		hash_destruir(tp->hash_pokemones);
		free(tp->jugador1);
		free(tp);
		return NULL;
	}
	tp->jugador2->pista = NULL;
	tp->jugador2->pokemon = NULL;

	char line[256];
	while (fgets(line, sizeof(line), file)) {
		struct pokemon_info *pokemon =
			malloc(sizeof(struct pokemon_info));
		if (!pokemon) {
			fclose(file);
			hash_destruir(tp->hash_pokemones);
			tp_destruir(tp);
			return NULL;
		}

		char *token = strtok(line, ",");
		if (!token) {
			free(pokemon);
			fclose(file);
			tp_destruir(tp);
			return NULL;
		}
		pokemon->nombre = malloc(strlen(token) + 1);
		if (!pokemon->nombre) {
			free(pokemon);
			fclose(file);
			tp_destruir(tp);
			return NULL;
		}
		strcpy(pokemon->nombre, token);

		token = strtok(NULL, ",");
		if (!token) {
			free(pokemon->nombre);
			free(pokemon);
			fclose(file);
			tp_destruir(tp);
			return NULL;
		}
		pokemon->fuerza = atoi(token);

		token = strtok(NULL, ",");
		if (!token) {
			free(pokemon->nombre);
			free(pokemon);
			fclose(file);
			tp_destruir(tp);
			return NULL;
		}
		pokemon->destreza = atoi(token);

		token = strtok(NULL, ",");
		if (!token) {
			free(pokemon->nombre);
			free(pokemon);
			fclose(file);
			tp_destruir(tp);
			return NULL;
		}
		pokemon->inteligencia = atoi(token);

		hash_insertar(tp->hash_pokemones, pokemon->nombre, pokemon,
			      NULL);
	}

	fclose(file);
	return tp;
}

int tp_cantidad_pokemon(TP *tp)
{
	if (!tp || !tp->hash_pokemones)
		return -1;
	return (int)hash_cantidad(tp->hash_pokemones);
}
void capitalizar_primera_letra(char *cadena)
{
	if (cadena == NULL || strlen(cadena) == 0) {
		return;
	}

	cadena[0] = (char)toupper(cadena[0]);
}
const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre)
{
	if (!tp || !tp->hash_pokemones || !nombre)
		return NULL;
	char nombre_modificable[strlen(nombre) + 1];
	strcpy(nombre_modificable, nombre);
	capitalizar_primera_letra(nombre_modificable);
	return hash_obtener(tp->hash_pokemones, nombre_modificable);
}

bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre)
{
	if (!tp || !tp->hash_pokemones || !nombre ||
	    (jugador != JUGADOR_1 && jugador != JUGADOR_2)) {
		return false;
	}

	struct pokemon_info *pokemon = hash_obtener(tp->hash_pokemones, nombre);
	if (!pokemon) {
		return false;
	}
	struct jugador *jugador_actual =
		asignar_jugador(tp, jugador_actual, jugador);
	struct jugador *otro_jugador;
	if (jugador == JUGADOR_1) {
		otro_jugador = tp->jugador2;
	} else {
		otro_jugador = tp->jugador1;
	}

	if (otro_jugador && otro_jugador->pokemon &&
	    strcmp(otro_jugador->pokemon->nombre, nombre) == 0) {
		return false;
	}

	if (jugador_actual->pokemon) {
		free(jugador_actual->pokemon->nombre);
		free(jugador_actual->pokemon);
	}

	jugador_actual->pokemon = malloc(sizeof(struct pokemon_info));
	if (!jugador_actual->pokemon) {
		return false;
	}

	jugador_actual->pokemon->nombre = copiar_nombre(pokemon->nombre);
	if (!jugador_actual->pokemon->nombre) {
		free(jugador_actual->pokemon);
		return false;
	}

	jugador_actual->pokemon->fuerza = pokemon->fuerza;
	jugador_actual->pokemon->destreza = pokemon->destreza;
	jugador_actual->pokemon->inteligencia = pokemon->inteligencia;

	return true;
}

char *tp_nombres_disponibles(TP *tp)
{
	if (!tp || !tp->hash_pokemones) {
		return NULL;
	}

	nombres_t nombres;
	nombres.nombres = malloc(10 * sizeof(char *));
	if (!nombres.nombres) {
		return NULL;
	}
	nombres.cantidad = 0;
	nombres.capacidad = 10;
	nombres.tp = tp;

	hash_con_cada_clave(tp->hash_pokemones, agregar_clave, &nombres);

	qsort(nombres.nombres, nombres.cantidad, sizeof(char *), comparar);

	size_t total_length = 0;
	for (size_t i = 0; i < nombres.cantidad; i++) {
		total_length += strlen(nombres.nombres[i]) + 1;
	}

	char *resultado = malloc(total_length + 1);
	if (!resultado) {
		for (size_t i = 0; i < nombres.cantidad; i++) {
			free(nombres.nombres[i]);
		}
		free(nombres.nombres);
		return NULL;
	}

	resultado[0] = '\0';
	for (size_t i = 0; i < nombres.cantidad; i++) {
		strcat(resultado, nombres.nombres[i]);
		if (i < nombres.cantidad - 1) {
			strcat(resultado, ",");
		}
		free(nombres.nombres[i]);
	}
	free(nombres.nombres);
	return resultado;
}

unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return 0;

	struct jugador *jugador_actual =
		asignar_jugador(tp, jugador_actual, jugador);

	if (!jugador_actual->pista || lista_vacia(jugador_actual->pista))
		return 0;

	if (posicion >= lista_tamanio(jugador_actual->pista))
		return 0;

	lista_quitar_de_posicion(jugador_actual->pista, posicion);

	return (unsigned)lista_tamanio(jugador_actual->pista);
}

unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return 0;

	struct jugador *jugador_actual =
		asignar_jugador(tp, jugador_actual, jugador);

	if (!jugador_actual->pista) {
		jugador_actual->pista = lista_crear();
		if (!jugador_actual->pista)
			return 0;
	}

	unsigned total_obstaculos =
		(unsigned)lista_tamanio(jugador_actual->pista);

	if (posicion >= total_obstaculos) {
		lista_insertar(jugador_actual->pista, (void *)obstaculo);
	} else {
		lista_insertar_en_posicion(jugador_actual->pista,
					   (void *)obstaculo, posicion);
	}

	return (unsigned)lista_tamanio(jugador_actual->pista);
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp,
						   enum TP_JUGADOR jugador)
{
	if (!tp || !tp->hash_pokemones) {
		return NULL;
	}

	if (jugador == JUGADOR_1) {
		return tp->jugador1->pokemon;
	} else if (jugador == JUGADOR_2) {
		return tp->jugador2->pokemon;
	}

	return NULL;
}

char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp) {
		return NULL;
	}

	struct jugador *jugador_actual = asignar_jugador(tp, NULL, jugador);

	if (!jugador_actual || !jugador_actual->pista) {
		return NULL;
	}

	char *obstaculos_pista =
		malloc(lista_tamanio(jugador_actual->pista) + 1);
	if (!obstaculos_pista) {
		return NULL;
	}

	obstaculos_pista_aux_t aux = { .obstaculos_pista = obstaculos_pista,
				       .posicion = 0 };
	lista_con_cada_elemento(jugador_actual->pista,
				agregar_obstaculo_a_string, &aux);
	obstaculos_pista[aux.posicion] = '\0';

	return obstaculos_pista;
}
void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return;

	struct jugador *jugador_actual =
		asignar_jugador(tp, jugador_actual, jugador);

	if (jugador_actual->pista) {
		lista_destruir(jugador_actual->pista);
		jugador_actual->pista = NULL;
	}
}

unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return 0;
	struct jugador *jugador_actual =
		asignar_jugador(tp, jugador_actual, jugador);
	if (!jugador_actual->pokemon || !jugador_actual->pista ||
	    lista_vacia(jugador_actual->pista))
		return 0;
	unsigned tiempo = 0;
	lista_iterador_t *iterador =
		lista_iterador_crear(jugador_actual->pista);
	if (!iterador)
		return 0;
	while (lista_iterador_tiene_siguiente(iterador)) {
		enum TP_OBSTACULO obstaculo =
			(enum TP_OBSTACULO)lista_iterador_elemento_actual(
				iterador);
		switch (obstaculo) {
		case OBSTACULO_FUERZA:
			tiempo += (unsigned)abs(
				(int)(10 - jugador_actual->pokemon->fuerza));
			break;
		case OBSTACULO_DESTREZA:
			tiempo += (unsigned)abs(
				(int)(10 - jugador_actual->pokemon->destreza));
			break;
		case OBSTACULO_INTELIGENCIA:
			tiempo += (unsigned)abs(
				(int)(10 -
				      jugador_actual->pokemon->inteligencia));
			break;
		default:
			lista_iterador_destruir(iterador);
			return 0;
		}
		lista_iterador_avanzar(iterador);
	}
	lista_iterador_destruir(iterador);

	return tiempo;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return NULL;

	struct jugador *jug = asignar_jugador(tp, jug, jugador);

	if (!jug || !jug->pokemon || !jug->pista ||
	    lista_vacia(jug->pista) == true)
		return NULL;

	size_t num_obstaculos = lista_tamanio(jug->pista);
	if (num_obstaculos == 0)
		return NULL;

	char *csv_tiempo = malloc((num_obstaculos * 5 + 1) * sizeof(char));
	if (!csv_tiempo)
		return NULL;
	csv_tiempo[0] = '\0';

	lista_iterador_t *iterador = lista_iterador_crear(jug->pista);
	if (!iterador) {
		free(csv_tiempo);
		return NULL;
	}
	int i = 0;
	while (lista_iterador_tiene_siguiente(iterador)) {
		i++;
		enum TP_OBSTACULO obstaculo =
			(enum TP_OBSTACULO)lista_iterador_elemento_actual(
				iterador);
		int tiempo = 0;
		switch ((enum TP_OBSTACULO)obstaculo) {
		case OBSTACULO_FUERZA:
			tiempo += abs((int)(10 - jug->pokemon->fuerza));
			break;
		case OBSTACULO_DESTREZA:
			tiempo += abs((int)(10 - jug->pokemon->destreza));
			break;
		case OBSTACULO_INTELIGENCIA:
			tiempo += abs((int)(10 - jug->pokemon->inteligencia));
			break;
		default:
			lista_iterador_destruir(iterador);
			free(csv_tiempo);
			return NULL;
		}
		char str_tiempo[11];
		sprintf(str_tiempo, "%d", tiempo);
		if (i > 1) {
			strcat(csv_tiempo, ",");
		}

		strcat(csv_tiempo, str_tiempo);

		lista_iterador_avanzar(iterador);
	}

	lista_iterador_destruir(iterador);

	return csv_tiempo;
}
void tp_liberar_nombres(char *nombres)
{
	free(nombres);
}

void tp_destruir(TP *tp)
{
	if (tp) {
		if (tp->hash_pokemones) {
			hash_con_cada_clave(tp->hash_pokemones, liberar, NULL);
			hash_destruir(tp->hash_pokemones);
		}
		liberar_jugador(tp->jugador1);
		liberar_jugador(tp->jugador2);
		free(tp);
	}
}
