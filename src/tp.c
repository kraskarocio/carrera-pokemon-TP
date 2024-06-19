#include "tp.h"
#include <stdlib.h>

typedef struct tp {
    hash_t *hash_pokemones;
    struct pokemon_info *pokemon_1;
    lista_t *pista_1;
    struct pokemon_info *pokemon_2;
    lista_t *pista_2;
} TP;


TP *tp_crear(const char *nombre_archivo)
{
	return NULL;
}

int tp_cantidad_pokemon(TP *tp)
{
	return 0;
}

const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre)
{
	return NULL;
}

char *tp_nombres_disponibles(TP *tp)
{
	return NULL;
}

bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre)
{
	return false;
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp,
						   enum TP_JUGADOR jugador)
{
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

void tp_destruir(TP *tp)
{
	return;
}