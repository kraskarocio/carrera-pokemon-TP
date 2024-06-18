#ifndef __TP_H__
#define __TP_H__
#include <stdbool.h>

#define IDENTIFICADOR_OBSTACULO_FUERZA 'F'
#define IDENTIFICADOR_OBSTACULO_DESTREZA 'D'
#define IDENTIFICADOR_OBSTACULO_INTELIGENCIA 'I'
#include "lista.h"
#include "hash.h"
#include "quicksort.h"


enum TP_JUGADOR { JUGADOR_1, JUGADOR_2 };

enum TP_OBSTACULO {
	OBSTACULO_FUERZA,
	OBSTACULO_DESTREZA,
	OBSTACULO_INTELIGENCIA
};

struct pokemon_info {
	char *nombre;
	int fuerza, destreza, inteligencia;
};

struct TP {
	hash_t *hash_pokemones;
	struct jugador *jugador1;
	struct jugador *jugador2;
};

struct jugador {
	struct pokemon_info *pokemon;
	lista_t *pista;
};

/**
 * Crea un nuevo TP a partir de un archivo de texto.
 * El archivo de texto tiene información de los pokemon con el siguiente formato:
 * 
 * nombre1,fuerza1,destreza1,inteligencia1
 * nombre2,fuerza2,destreza2,inteligencia2
 * ...
 * nombreN,fuerzaN,destrezaN,inteligenciaN
*/
struct TP *tp_crear(const char *nombre_archivo);

/**
 * Devuelve la cantidad de pokemon existentes.
*/
int tp_cantidad_pokemon(struct TP *tp);

/**
 * Busca un pokemon por nombre. Si no lo encuentra devuelve NULL.
 * El nombre del pokemon puede estar en mayúsculas o minúsculas y el resultado debería ser el mismo.
 * Por ejemplo "Pikachu" y "pikachu" deberían devolver el mismo resultado (si es que existe).
*/
const struct pokemon_info *tp_buscar_pokemon(const struct TP *tp,
					     const char *nombre);

/**
 * Devuelve un string con los nombres de los pokemon disponibles.
 * Los nombres deben ser devueltos en orden alfabético y separados por coma.
 * El string devuelto debe ser liberado con free por quien invoca esta función.
*/
char *tp_nombres_disponibles(const struct TP *tp);

/**
 * Selecciona un pokemon para un jugador.
 * Si el jugador ya tiene un pokemon seleccionado, se reemplaza por el nuevo.
 * Un jugador no puede seleccionar el mismo pokemon que el otro jugador.
 * Devuelve true si el pokemon fue seleccionado correctamente, false en caso contrario.
*/
bool tp_seleccionar_pokemon(const struct TP *tp, enum TP_JUGADOR jugador,
			    const char *nombre);

/**
 * Devuelve el pokemon seleccionado por un jugador.
 * Si el jugador no tiene un pokemon seleccionado, devuelve NULL.
*/
const struct pokemon_info *tp_pokemon_seleccionado(const struct TP *tp,
						   enum TP_JUGADOR jugador);

/**
 * Agrega un obstáculo en la pista del jugador en la posición indicada.
 * Si ya hay un obstáculo en esa posición, este es desplazado a la siguiente posición.
 * Si la posición es mayor a la longitud de la pista, el obstáculo se agrega al final.
 * Devuelve la cantidad total de obstáculos en la pista del jugador o 0 si no se pudo agregar el obstáculo.
*/
unsigned tp_agregar_obstaculo(const struct TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion);

/**
 * Quita un obstáculo de la pista del jugador en la posición indicada.
 * Devuelve la cantidad total de obstáculos en la pista del jugador o 0 si no se pudo quitar el obstáculo.
*/
unsigned tp_quitar_obstaculo(const struct TP *tp, enum TP_JUGADOR jugador,
			     unsigned posicion);

/**
 * Devuelve un string con los obstáculos en la pista del jugador en orden.
 * Cada obstáculo debe ser representado por una letra: 'F' para fuerza, 'D' para destreza e 'I' para inteligencia.
 * El string devuelto debe ser liberado con free por quien invoca esta función.
 * Si el jugador no tiene obstáculos en la pista o en caso de error devuelve NULL.
*/
char *tp_obstaculos_pista(const struct TP *tp, enum TP_JUGADOR jugador);

/**
 * Quita todos los obstáculos de la pista del jugador.
*/
void tp_limpiar_pista(const struct TP *tp, enum TP_JUGADOR jugador);

/**
 * Devuelve las unidades de tiempo que tarda el pokemon del jugador en completar la pista de obstáculos
 * en función de sus habilidades y los obstáculos en la pista.
 * Si el jugador no tiene un pokemon seleccionado o no hay obstáculos en la pista, devuelve 0.
*/
unsigned tp_calcular_tiempo_pista(const struct TP *tp, enum TP_JUGADOR jugador);

/**
 * Devuelve un string csv con el tiempo que tarda el pokemon del jugador en completar cada obstáculo de la pista.
 * Si el jugador no tiene un pokemon seleccionado o no hay obstáculos en la pista, devuelve NULL.
 * El string devuelto debe ser liberado con free por quien invoca esta función.
*/
char *tp_tiempo_por_obstaculo(const struct TP *tp, enum TP_JUGADOR jugador);

/**
 * Libera toda la memoria reservada por el TP.
*/
void tp_destruir(struct TP *tp);

#endif // __TP_H__