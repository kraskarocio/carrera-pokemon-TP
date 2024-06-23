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
void imprimir_pokemon(char *nombre);
/*
Diseño que se imprime cuando termina el juego, ya sea porque se gano o se perdio.
*/
void salida();
void imprimir_poke_tabla(int id, char *nombre, int fuerza, int inteligencia,
			  int destreza);
void imprimir_opciones_obstaculos(int pocicion);
void imprimir_dificultades(int obs_facil, int reintentos_facil, int porcentaje_facil, int pista_facil, 
int obs_normal, int reintentos_normal, int porcentaje_normal, int pista_normal, int obs_dificil, 
int reintentos_dificil, int porcentaje_dificil, int pista_dificil, int obs_imposible, 
int reintentos_imposible, int porcentaje_imposible, int pista_imposible);
void versus(char *nombre1, char *nombre2, int fuerza1, int fuerza2,
	    int destreza1, int destreza2, int inteligencia1,
	    int inteligencia2);
void pistas(int dificultad, char *pista);
void fin_de_la_partida(bool ganada, int cantidad_de_rondas_por_jugar,
		    float porcentaje, int tiempo1, int tiempo2);
#endif