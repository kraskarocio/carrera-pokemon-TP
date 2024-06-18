#ifndef MENU_H
#define MENU_H
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ANSI-color-codes.h"
#include "ASCII_print.h"
#include "tp.h"

struct TP;

enum DIFICULTAD { FACIL, DIFICIL, IMPOSIBLE };
typedef struct menu menu_t;
menu_t *menu_crear();
/**
 * Muestra el menú principal del juego.
 * Se le da la bienvenida al jugador y se le da la opción de comenzar a jugar o salir.
 * Si el jugador decide comenzar a jugar devuelve 1, si decide salir devuelve 2, y
 * si el jugador ingresa una opción inválida, devuelve 3.
*/
int mostrar_menu_principal(struct TP *tp, menu_t *menu, char opcion);
/**
 * Muestra el menú de dificultad.
 * El usuario puede elegir entre las dificultades FACIL, DIFICIL e IMPOSIBLE.
 * Si el jugador elige una opción inválida, devuelve true.
*/
bool elegir_dificultad(menu_t *menu, char opcion, int cant_rondas_facil,
		       int cant_rondas_dificil, int cant_rondas_imposible,
		       int *rondas_por_jugar);

char *seleccionar_pokemon_al_azar(struct TP *tp);
/**
 * Muestra cada uno de los pokemones disponibles para que el jugador elija uno.
 * Estos se ven por pantalla en forma de tabla. Y con su ascii art correspondiente.
 * En caso de que no lo tenga, se imprime una pokebola.
 * Los pokemones se mostrarán uno por uno, y el jugador deberá decidir si ir para atrás, salir o elegir ese pokemon.
*/
char *seleccionar_pokemon(struct TP *tp);
char *crear_pista_al_azar(struct TP *tp, menu_t *menu);
void crear_pista(struct TP *tp, menu_t *menu);
//void mostar_carrera(struct TP *tp);
int cantidad_de_rondas_jugadas(menu_t *menu);
bool finalizo_carrera(menu_t *menu);
void mostrar_puntaje_jugador(struct TP *tp, menu_t *menu);
void menu_destruir(menu_t *menu);

#endif
