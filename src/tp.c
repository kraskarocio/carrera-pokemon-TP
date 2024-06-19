#include "tp.h"
#include <stdlib.h>
#include <ctype.h>
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

	cadena[0] = (char)toupper(cadena[0]);
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

TP *tp_crear(const char *nombre_archivo) {
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

    tp->pista_1 = lista_crear();
    if (!tp->pista_1) {
        fclose(file);
        hash_destruir(tp->hash_pokemones);
        free(tp);
        return NULL;
    }

    tp->pista_2 = lista_crear();
    if (!tp->pista_2) {
        fclose(file);
        hash_destruir(tp->hash_pokemones);
        lista_destruir(tp->pista_1);
        free(tp);
        return NULL;
    }

    tp->pokemon_1 = malloc(sizeof(struct pokemon_info));
    if (!tp->pokemon_1) {
        fclose(file);
        hash_destruir(tp->hash_pokemones);
        lista_destruir(tp->pista_1);
        lista_destruir(tp->pista_2);
        free(tp);
        return NULL;
    }
	tp->pokemon_1->nombre = NULL;
	tp->pokemon_1->fuerza = 0;
	tp->pokemon_1->destreza = 0;
	tp->pokemon_1->inteligencia = 0;


    tp->pokemon_2 = malloc(sizeof(struct pokemon_info));
    if (!tp->pokemon_2) {
        fclose(file);
        hash_destruir(tp->hash_pokemones);
        lista_destruir(tp->pista_1);
        lista_destruir(tp->pista_2);
        free(tp->pokemon_1);
        free(tp);
        return NULL;
    }
	tp->pokemon_2->nombre = NULL;
	tp->pokemon_2->fuerza = 0;
	tp->pokemon_2->destreza = 0;
	tp->pokemon_2->inteligencia = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct pokemon_info *pokemon = malloc(sizeof(struct pokemon_info));
        if (!pokemon) {
            fclose(file);
            lista_destruir(tp->pista_1);
            lista_destruir(tp->pista_2);
            free(tp->pokemon_1);
            free(tp->pokemon_2);
            hash_destruir(tp->hash_pokemones);
            free(tp);
            return NULL;
        }

        char *token = strtok(line, ",");
        if (!token) {
            free(pokemon);
            fclose(file);
            lista_destruir(tp->pista_1);
            lista_destruir(tp->pista_2);
            free(tp->pokemon_1);
            free(tp->pokemon_2);
            hash_destruir(tp->hash_pokemones);
            free(tp);
            return NULL;
        }

        pokemon->nombre = malloc(strlen(token) + 1);
        if (!pokemon->nombre) {
            free(pokemon);
            fclose(file);
            lista_destruir(tp->pista_1);
            lista_destruir(tp->pista_2);
            free(tp->pokemon_1);
            free(tp->pokemon_2);
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
            lista_destruir(tp->pista_1);
            lista_destruir(tp->pista_2);
            free(tp->pokemon_1);
            free(tp->pokemon_2);
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
            lista_destruir(tp->pista_1);
            lista_destruir(tp->pista_2);
            free(tp->pokemon_1);
            free(tp->pokemon_2);
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
            lista_destruir(tp->pista_1);
            lista_destruir(tp->pista_2);
            free(tp->pokemon_1);
            free(tp->pokemon_2);
            hash_destruir(tp->hash_pokemones);
            free(tp);
            return NULL;
        }

        pokemon->inteligencia = atoi(token);

        hash_insertar(tp->hash_pokemones, pokemon->nombre, pokemon, NULL);
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
	if ((tp_pokemon_seleccionado((TP *)tp, JUGADOR_1)&& 
		 tp_pokemon_seleccionado((TP *)tp, JUGADOR_1)->nombre&&
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

    struct pokemon_info *pokemon = hash_obtener(tp->hash_pokemones, nombre);
    if (!pokemon) {
        return false;
    }

    struct pokemon_info **jugador_actual;
    struct pokemon_info **otro_jugador;
    
    if (jugador == JUGADOR_1) {
        jugador_actual = &tp->pokemon_1;
        otro_jugador = &tp->pokemon_2;
    } else {
        jugador_actual = &tp->pokemon_2;
        otro_jugador = &tp->pokemon_1;
    }

    if (*otro_jugador && (*otro_jugador)->nombre && strcmp((*otro_jugador)->nombre, nombre) == 0) {
    	return false;
	}
    if (*jugador_actual) {
        free((*jugador_actual)->nombre);
        free(*jugador_actual);
    }

    *jugador_actual = malloc(sizeof(struct pokemon_info));
    if (!(*jugador_actual)) {
        return false;
    }

    (*jugador_actual)->nombre = copiar_nombre(pokemon->nombre);
    if (!(*jugador_actual)->nombre) {
        free(*jugador_actual);
        return false;
    }

    (*jugador_actual)->fuerza = pokemon->fuerza;
    (*jugador_actual)->destreza = pokemon->destreza;
    (*jugador_actual)->inteligencia = pokemon->inteligencia;

    return true;
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp, enum TP_JUGADOR jugador)
{
    if (!tp || !tp->hash_pokemones) {
        return NULL;
    }

    if (jugador == JUGADOR_1) {
        return tp->pokemon_1;
    } else if (jugador == JUGADOR_2) {
        return tp->pokemon_2;
    }

    return NULL;
}


unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion)
{
	return 0;
}

unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion)
{
	return 0;
}

char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{
	return NULL;
}

void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	return;
}

unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	return 0;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	return NULL;
}
bool liberar(const char *clave, void *valor, void *aux)
{
	struct pokemon_info *pokemon = (struct pokemon_info *)valor;
	free(pokemon->nombre);
	free(pokemon);
	return true;
}

void tp_destruir(TP *tp)
{
	if (tp) {
		if (tp->hash_pokemones) {
			hash_con_cada_clave(tp->hash_pokemones, liberar, NULL);
			hash_destruir(tp->hash_pokemones);
		}
		if (tp->pokemon_1) {
			free(tp->pokemon_1->nombre);
			free(tp->pokemon_1);
		}
		if (tp->pista_1) {
			lista_destruir(tp->pista_1);
		}
		if (tp->pokemon_2) {
			free(tp->pokemon_2->nombre);
			free(tp->pokemon_2);
		}
		if (tp->pista_2) {
			lista_destruir(tp->pista_2);
		}
		free(tp);
	}
}
