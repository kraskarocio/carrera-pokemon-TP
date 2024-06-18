#include "tp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 100
#define BUFFER_SIZE_MINI 6

/*******************************************************************
 *               Funciones y estructuras auxiliares
 ******************************************************************/

// Devuelve una copia de la cadena.
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
// Estructura que guarda los nombres de los pokemones.
// Se utiliza para poder ordenarlos alfabéticamente. En la función tp_nombres_disponibles.
typedef struct {
	char **nombres;
	size_t cantidad;
	size_t capacidad;
	const struct TP *tp;
} nombres_t;

// Agrega una clave al arreglo de nombres.
// Devuelve false si no se pudo agregar la clave.
// Se usa en la función hash_con_cada_clave.
// Si la clave coincide con un nombre de un pokemon ya elegido, no se agrega.
bool agregar_clave(const char *clave, void *valor, void *aux)
{
	nombres_t *nombres = (nombres_t *)aux;
	const struct TP *tp = nombres->tp;
	if ((tp_pokemon_seleccionado(tp, JUGADOR_1) &&
	     strcmp(tp_pokemon_seleccionado(tp, JUGADOR_1)->nombre, clave) ==
		     0) ||
	    (tp_pokemon_seleccionado(tp, JUGADOR_2) &&
	     strcmp(tp_pokemon_seleccionado(tp, JUGADOR_2)->nombre, clave) ==
		     0)) {
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

// Compara dos cadenas. Es utilizada en la función qsort.
int comparar(const void *a, const void *b)
{
	return strcmp(*(const char **)a, *(const char **)b);
}

// Estructura auxiliar para agregar un obstáculo a un string.
// Se utiliza en la función lista_con_cada_elemento.
typedef struct {
	char *obstaculos_pista;
	size_t posicion;
} obstaculos_pista_aux_t;

// Agrega un obstáculo a un string.
// Devuelve false si no se pudo agregar el obstáculo.
// Se usa en la función lista_con_cada_elemento.
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

// Estructura auxiliar para liberar los nombres de los pokemones ubicados en el hash.
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
// Asigna el jugador actual. Si el jugador actual es JUGADOR_1, devuelve el jugador1, sino el jugador2.
struct jugador *asignar_jugador(const struct TP *tp,
				struct jugador *jugador_actual,
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

struct TP *tp_crear(const char *nombre_archivo)
{
	if (!nombre_archivo)
		return NULL;

	FILE *file = fopen(nombre_archivo, "r");
	if (!file)
		return NULL;

	struct TP *tp = malloc(sizeof(struct TP));
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
			hash_destruir(tp->hash_pokemones);
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

int tp_cantidad_pokemon(struct TP *tp)
{
	if (!tp || !tp->hash_pokemones)
		return 0;
	return (int)hash_cantidad(tp->hash_pokemones);
}

const struct pokemon_info *tp_buscar_pokemon(const struct TP *tp,
					     const char *nombre)
{
	if (!tp || !tp->hash_pokemones || !nombre)
		return NULL;
	size_t len = strlen(nombre);
	char *nombre_modificado = malloc(len + 1);
	if (!nombre_modificado)
		return NULL;
	if (len > 0) {
		nombre_modificado[0] = (char)toupper(nombre[0]);
		for (size_t i = 1; i < len; i++) {
			nombre_modificado[i] = (char)tolower(nombre[i]);
		}
	}
	nombre_modificado[len] = '\0';

	struct pokemon_info *pokemon =
		hash_obtener(tp->hash_pokemones, nombre_modificado);

	free(nombre_modificado);

	return pokemon;
}

char *tp_nombres_disponibles(const struct TP *tp)
{
	if (!tp || !tp->hash_pokemones) {
		return NULL;
	}

	size_t cantidad_inicial = 10;
	nombres_t nombres;
	nombres.nombres = malloc(cantidad_inicial * sizeof(char *));
	nombres.cantidad = 0;
	nombres.capacidad = cantidad_inicial;
	nombres.tp = tp;
	if (!nombres.nombres) {
		return NULL;
	}

	hash_con_cada_clave(tp->hash_pokemones, agregar_clave, &nombres);

	if (nombres.cantidad == 0) {
		free(nombres.nombres);
		return NULL;
	}

	quicksort_alfabetico(nombres.nombres, nombres.cantidad);

	size_t longitud_total = 0;
	for (size_t i = 0; i < nombres.cantidad; i++) {
		longitud_total += strlen(nombres.nombres[i]) + 1;
	}

	char *resultado = malloc(longitud_total + 1);
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

bool tp_seleccionar_pokemon(const struct TP *tp, enum TP_JUGADOR jugador,
			    const char *nombre)
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

const struct pokemon_info *tp_pokemon_seleccionado(const struct TP *tp,
						   enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2)) {
		return NULL;
	}

	struct jugador *jugador_seleccionado =
		asignar_jugador(tp, jugador_seleccionado, jugador);
	if (!jugador_seleccionado->pokemon) {
		return NULL;
	}

	return jugador_seleccionado->pokemon;
}

unsigned tp_agregar_obstaculo(const struct TP *tp, enum TP_JUGADOR jugador,
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

unsigned tp_quitar_obstaculo(const struct TP *tp, enum TP_JUGADOR jugador,
			     unsigned posicion)
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

char *tp_obstaculos_pista(const struct TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2))
		return NULL;

	struct jugador *jugador_actual =
		asignar_jugador(tp, jugador_actual, jugador);

	if (!jugador_actual->pista || lista_vacia(jugador_actual->pista))
		return NULL;

	size_t tamanio_pista = lista_tamanio(jugador_actual->pista);
	char *obstaculos_pista = calloc(
		tamanio_pista + 2,
		sizeof(char)); // Agregamos 1 para el carácter nulo y 1 para un espacio adicional
	if (!obstaculos_pista)
		return NULL;

	obstaculos_pista_aux_t aux = { .obstaculos_pista = obstaculos_pista,
				       .posicion = 0 };

	lista_con_cada_elemento(jugador_actual->pista,
				agregar_obstaculo_a_string, &aux);
	obstaculos_pista[tamanio_pista] = '\0';
	return obstaculos_pista;
}

void tp_limpiar_pista(const struct TP *tp, enum TP_JUGADOR jugador)
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

unsigned tp_calcular_tiempo_pista(const struct TP *tp, enum TP_JUGADOR jugador)
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

char *tp_tiempo_por_obstaculo(const struct TP *tp, enum TP_JUGADOR jugador)
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

void tp_destruir(struct TP *tp)
{
	if (!tp)
		return;

	hash_con_cada_clave(tp->hash_pokemones, liberar, NULL);
	hash_destruir(tp->hash_pokemones);
	liberar_jugador(tp->jugador1);
	liberar_jugador(tp->jugador2);
	free(tp);
}