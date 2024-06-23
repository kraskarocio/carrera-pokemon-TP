#include "tp.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#include "lista.h"
#include "quicksort.h"

struct tp {
	hash_t *hash_pokemones;
	struct pokemon_info *pokemon_1;
	lista_t *pista_1;
	struct pokemon_info *pokemon_2;
	lista_t *pista_2;
};

void capitalizar_primera_letra(char *cadena)
{
	if (cadena == NULL || strlen(cadena) == 0) {
		return;
	}

	cadena[0] = (char)toupper((unsigned char)cadena[0]);

	for (int i = 1; cadena[i] != '\0'; i++) {
		cadena[i] = (char)tolower((unsigned char)cadena[i]);
	}
}

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

TP *tp_crear(const char *nombre_archivo)
{
	if (!nombre_archivo) {
		return NULL;
	}

	FILE *file = fopen(nombre_archivo, "r");
	if (!file) {
		return NULL;
	}

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

	tp->pista_1 = NULL;

	tp->pista_2 = NULL;
	tp->pokemon_1 = NULL;

	tp->pokemon_2 = NULL;

	char line[256];
	while (fgets(line, sizeof(line), file)) {
		struct pokemon_info *pokemon =
			malloc(sizeof(struct pokemon_info));
		if (!pokemon) {
			fclose(file);

			hash_destruir(tp->hash_pokemones);
			free(tp);
			return NULL;
		}

		char *token = strtok(line, ",");
		if (!token) {
			free(pokemon);
			fclose(file);

			hash_destruir(tp->hash_pokemones);
			free(tp);
			return NULL;
		}

		pokemon->nombre = malloc(strlen(token) + 1);
		if (!pokemon->nombre) {
			free(pokemon);
			fclose(file);

			hash_destruir(tp->hash_pokemones);
			free(tp);
			return NULL;
		}

		strcpy(pokemon->nombre, token);

		token = strtok(NULL, ",");
		if (!token) {
			free(pokemon->nombre);
			free(pokemon);
			fclose(file);

			hash_destruir(tp->hash_pokemones);
			free(tp);
			return NULL;
		}

		pokemon->fuerza = atoi(token);

		token = strtok(NULL, ",");
		if (!token) {
			free(pokemon->nombre);
			free(pokemon);
			fclose(file);

			hash_destruir(tp->hash_pokemones);
			free(tp);
			return NULL;
		}

		pokemon->destreza = atoi(token);

		token = strtok(NULL, ",");
		if (!token) {
			free(pokemon->nombre);
			free(pokemon);
			fclose(file);

			hash_destruir(tp->hash_pokemones);
			free(tp);
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

const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre)
{
	if (!tp || !tp->hash_pokemones || !nombre)
		return NULL;
	char nombre_modificable[strlen(nombre) + 1];
	strcpy(nombre_modificable, nombre);
	capitalizar_primera_letra(nombre_modificable);
	return hash_obtener(tp->hash_pokemones, nombre_modificable);
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
	// hacer mas lindo
	const TP *tp = nombres->tp;
	if ((tp_pokemon_seleccionado((TP *)tp, JUGADOR_1) &&
	     tp_pokemon_seleccionado((TP *)tp, JUGADOR_1)->nombre &&
	     strcmp(tp_pokemon_seleccionado((TP *)tp, JUGADOR_1)->nombre,
		    clave) == 0) ||
	    (tp_pokemon_seleccionado((TP *)tp, JUGADOR_2) &&
	     tp_pokemon_seleccionado((TP *)tp, JUGADOR_2)->nombre &&
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

	quicksort_alfabetico(nombres.nombres, nombres.cantidad);

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

bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre)
{
	if (!tp || !tp->hash_pokemones || !nombre ||
	    (jugador != JUGADOR_1 && jugador != JUGADOR_2)) {
		return false;
	}
	char *c_aux;
	c_aux = malloc(strlen(nombre) + 1);
	strcpy(c_aux, nombre);
	capitalizar_primera_letra(c_aux);
	struct pokemon_info *pokemon = hash_obtener(tp->hash_pokemones, c_aux);
	if (pokemon == NULL) {
		free(c_aux);
		return false;
	}

	struct pokemon_info **poke_jugador_actual;
	struct pokemon_info **poke_otro_jugador;

	if (jugador == JUGADOR_1) {
		poke_jugador_actual = &tp->pokemon_1;
		poke_otro_jugador = &tp->pokemon_2;
	} else {
		poke_jugador_actual = &tp->pokemon_2;
		poke_otro_jugador = &tp->pokemon_1;
	}

	if (*poke_otro_jugador && (*poke_otro_jugador)->nombre &&
	    strcmp((*poke_otro_jugador)->nombre, nombre) == 0) {
		free(c_aux);
		return false;
	}
	if (*poke_jugador_actual != NULL) {
		if ((*poke_jugador_actual)->nombre != NULL) {
			free((*poke_jugador_actual)->nombre);
		}
		free(*poke_jugador_actual);
	}
	*poke_jugador_actual = pokemon;
	free(c_aux);
	return true;
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp,
						   enum TP_JUGADOR jugador)
{
	if (!tp || !tp->hash_pokemones) {
		return NULL;
	}

	struct pokemon_info *poke_sel = (jugador == JUGADOR_1) ? tp->pokemon_1 :
								 tp->pokemon_2;

	if (poke_sel) {
		return poke_sel;
	}
	return NULL;
}

unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2)) {
		return 0;
	}

	lista_t *pista_actual;
	if (jugador == JUGADOR_1) {
		if (!tp->pista_1) {
			tp->pista_1 = lista_crear();
			if (!tp->pista_1) {
				return 0;
			}
		}
		pista_actual = tp->pista_1;
	} else {
		if (!tp->pista_2) {
			tp->pista_2 = lista_crear();
			if (!tp->pista_2) {
				printf("pista 2 null\n");
				return 0;
			}
		}
		pista_actual = tp->pista_2;
	}

	unsigned tamanio_pista = (unsigned)lista_tamanio(pista_actual);
	if (posicion > tamanio_pista)
		posicion = tamanio_pista;

	enum TP_OBSTACULO *nuevo_obstaculo = malloc(sizeof(enum TP_OBSTACULO));
	if (!nuevo_obstaculo)
		return 0;

	*nuevo_obstaculo = obstaculo;

	if (posicion == tamanio_pista) {
		if (!lista_insertar(pista_actual, nuevo_obstaculo)) {
			free(nuevo_obstaculo);
			return 0;
		}
	} else {
		if (!lista_insertar_en_posicion(pista_actual, nuevo_obstaculo,
						posicion)) {
			free(nuevo_obstaculo);
			return 0;
		}
	}

	return (unsigned)lista_tamanio(pista_actual);
}

unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion)
{
	if (!tp)
		return 0;

	lista_t *pista_actual;
	if (jugador == JUGADOR_1) {
		if (!tp->pista_1) {
			return 0;
		}
		pista_actual = tp->pista_1;
	} else {
		if (!tp->pista_2) {
			return 0;
		}
		pista_actual = tp->pista_2;
	}
	enum TP_OBSTACULO *obst =
		lista_quitar_de_posicion(pista_actual, posicion);
	if (obst == NULL)
		return 0;
	free(obst);
	obst = NULL;
	return (unsigned)lista_tamanio(pista_actual);
}

typedef struct {
	char *obstaculos_pista;
	size_t posicion;
} obstaculos_pista_aux_t;

bool agregar_obstaculo_a_string(void *obstaculo, void *aux)
{
	if (!obstaculo || !aux) {
		return false;
	}
	obstaculos_pista_aux_t *aux_data = (obstaculos_pista_aux_t *)aux;
	enum TP_OBSTACULO *obstaculo_actual = (enum TP_OBSTACULO *)obstaculo;
	if (!obstaculo_actual || !aux_data) {
		return false;
	}
	switch (*obstaculo_actual) {
	case OBSTACULO_FUERZA:
		aux_data->obstaculos_pista[aux_data->posicion++] =
			IDENTIFICADOR_OBSTACULO_FUERZA;
		break;
	case OBSTACULO_DESTREZA:
		aux_data->obstaculos_pista[aux_data->posicion++] =
			IDENTIFICADOR_OBSTACULO_DESTREZA;
		break;
	case OBSTACULO_INTELIGENCIA:
		aux_data->obstaculos_pista[aux_data->posicion++] =
			IDENTIFICADOR_OBSTACULO_INTELIGENCIA;
		break;
	default:
		return false;
	}
	return true;
}

char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2)) {
		return NULL;
	}

	lista_t *pista_actual;
	if (jugador == JUGADOR_1) {
		if (!tp->pista_1) {
			return NULL;
		}
		pista_actual = tp->pista_1;
	} else {
		if (!tp->pista_2 || lista_vacia(tp->pista_2)) {
			return NULL;
		}
		pista_actual = tp->pista_2;
	}

	char *obstaculos_pista = malloc(lista_tamanio(pista_actual) + 1);
	if (!obstaculos_pista) {
		return NULL;
	}

	obstaculos_pista_aux_t aux = { .obstaculos_pista = obstaculos_pista,
				       .posicion = 0 };
	lista_con_cada_elemento(pista_actual, agregar_obstaculo_a_string, &aux);
	obstaculos_pista[aux.posicion] = '\0';

	return obstaculos_pista;
}
void destruir_obstaculo(void *obstaculo)
{
	if (obstaculo != NULL) {
		free(obstaculo);
		obstaculo = NULL;
	}
}

void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp)
		return;

	lista_t *pista_actual = NULL;

	if (jugador == JUGADOR_1) {
		pista_actual = tp->pista_1;
		tp->pista_1 = NULL; // Prevent accidental reuse
	} else {
		pista_actual = tp->pista_2;
		tp->pista_2 = NULL; // Prevent accidental reuse
	}

	if (pista_actual) {
		lista_destruir_todo(pista_actual, destruir_obstaculo);
		pista_actual = NULL; // Ensure it's not used after freeing
	}
}

unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return 0;

	struct pokemon_info *pokemon;
	lista_t *pista;

	if (jugador == JUGADOR_1) {
		pokemon = tp->pokemon_1;
		pista = tp->pista_1;
	} else {
		pokemon = tp->pokemon_2;
		pista = tp->pista_2;
	}

	if (!pokemon || !pista) {
		return 0;
	}

	unsigned tiempo_final = 0;
	int tipo_obstaculo_previo = -1;
	int count = 0;

	size_t n_obstaculos = lista_tamanio(pista);
	if (n_obstaculos == 0) {
		return 0;
	}

	for (size_t i = 0; i < n_obstaculos; i++) {
		int *obstacle_t = (int *)lista_elemento_en_posicion(pista, i);
		if (!obstacle_t) {
			return 0;
		}
		int obstacle_type = *obstacle_t;

		int attribute_value;
		switch (obstacle_type) {
		case OBSTACULO_FUERZA:
			attribute_value = pokemon->fuerza;
			break;
		case OBSTACULO_DESTREZA:
			attribute_value = pokemon->destreza;
			break;
		case OBSTACULO_INTELIGENCIA:
			attribute_value = pokemon->inteligencia;
			break;
		default:
			continue;
		}

		int base_time = (10 - attribute_value);
		if (obstacle_type == tipo_obstaculo_previo) {
			base_time = (10 - count - attribute_value);
			count++;
		} else {
			count = 1;
		}

		int obstacle_time = base_time < 0 ? 0 : base_time;

		tiempo_final += (unsigned)obstacle_time;

		tipo_obstaculo_previo = obstacle_type;
	}

	return tiempo_final;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return NULL;

	struct pokemon_info *pokemon;
	lista_t *pista;
	if (jugador == JUGADOR_1) {
		pokemon = tp->pokemon_1;
		pista = tp->pista_1;
	} else {
		pokemon = tp->pokemon_2;
		pista = tp->pista_2;
	}

	size_t num_obstaculos = lista_tamanio(pista);
	if (num_obstaculos == 0)
		return NULL;

	char *csv_tiempo = malloc((num_obstaculos * 5 + 1) * sizeof(char));
	if (!csv_tiempo)
		return NULL;
	csv_tiempo[0] = '\0';

	int previous_obstacle_type = -1;
	int consecutive_count = 0;

	lista_iterador_t *iterador = lista_iterador_crear(pista);
	if (!iterador) {
		free(csv_tiempo);
		return NULL;
	}

	int i = 0;
	while (lista_iterador_tiene_siguiente(iterador)) {
		i++;
		enum TP_OBSTACULO *obstaculo =
			(enum TP_OBSTACULO *)lista_iterador_elemento_actual(
				iterador);
		printf("obstaculo: %d\n", *obstaculo);
		int attribute_value;
		switch (*obstaculo) {
		case OBSTACULO_FUERZA:
			attribute_value = pokemon->fuerza;
			break;
		case OBSTACULO_DESTREZA:
			attribute_value = pokemon->destreza;
			break;
		case OBSTACULO_INTELIGENCIA:
			attribute_value = pokemon->inteligencia;
			break;
		default:
			lista_iterador_destruir(iterador);
			free(csv_tiempo);
			return NULL;
		}

		int base_time = (10 - attribute_value);
		if (*obstaculo == previous_obstacle_type) {
			base_time = (10 - consecutive_count - attribute_value);
			consecutive_count++;
		} else {
			consecutive_count = 1;
		}

		int obstacle_time = base_time < 0 ? 0 : base_time;

		char str_tiempo[11];
		sprintf(str_tiempo, "%d", obstacle_time);
		if (i > 1) {
			strcat(csv_tiempo, ",");
		}

		strcat(csv_tiempo, str_tiempo);

		lista_iterador_avanzar(iterador);

		previous_obstacle_type = *obstaculo;
	}

	lista_iterador_destruir(iterador);
	return csv_tiempo;
}
bool liberar(const char *clave, void *valor, void *aux)
{
	struct pokemon_info *pokemon = (struct pokemon_info *)valor;
	if (pokemon != NULL) {
		if (pokemon->nombre != NULL) {
			free(pokemon->nombre);
		}
		free(pokemon);
	}

	pokemon = NULL;
	return true;
}

void tp_destruir(TP *tp)
{
	if (tp != NULL) {
		if (tp->hash_pokemones) {
			hash_con_cada_clave(tp->hash_pokemones, liberar, NULL);
			hash_destruir(tp->hash_pokemones);
		}
		if (tp->pokemon_1) {
			tp->pokemon_1 = NULL;
		}
		if (tp->pokemon_2) {
			tp->pokemon_2 = NULL;
		}
		tp_limpiar_pista(tp, JUGADOR_1);
		tp_limpiar_pista(tp, JUGADOR_2);
		free(tp);
	}
}