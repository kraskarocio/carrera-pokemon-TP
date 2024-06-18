#ifndef __ASCII_PRINT_H__
#define __ASCII_PRINT_H__
#include "ANSI-color-codes.h"
#define RESET "\x1b[0m"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void imprimir_menu_principal();
/*
Si el nombre del pokemon coincide con alguno de los pokemones de los cuales
tenemos ascii art, se imprime el ascii art correspondiente. Si no, se imprime
una pokebola.
*/
void pokemones(char *nombre);
void salida();
void imprimir_poke_tabla(int id, char *nombre, char *fuerza, char *inteligencia,
			 char *destreza);
void imprimir_dificultades(int obs_facil, int obs_dificil, int obs_imposible,
			   int reintentos_facil, int reintentos_dificil,
			   int reintentos_imposible, int porcentaje_facil,
			   int porcentaje_dificil, int porcentaje_imposible,
			   int pista_facil, int pista_dificil,
			   int pista_imposible);
void versus(char *nombre1, char *nombre2, char *fuerza1, char *fuerza2,
	    char *destreza1, char *destreza2, char *inteligencia1,
	    char *inteligencia2);
void pistas(int dificultad, char *pista, bool agregar);
void fin_de_la_partida(bool ganada, int cantidad_de_rondas_por_jugar,
		       char *porcentaje);
void puntajes_por_ronda(char *puntaje1, char *puntaje2, char *puntaje3);
#endif