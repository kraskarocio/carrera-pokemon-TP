#include "menu.h"

#define BOLD "\033[1m"
#define RESET "\x1b[0m"
#define ITALIC "\x1B[3m"
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NOMBRES 100
#define MAX_LONGITUD_NOMBRE 50
#define JUGADOR1 0
#define JUGADOR2 1
#define OBSTACULO_FUERZA 0
#define OBSTACULO_DESTREZA 1
#define OBSTACULO_INTELIGENCIA 2
#define JUGAR 1
#define SALIR 2
#define INCORRECTO 3

struct menu {
	enum DIFICULTAD dificultad;
};

/*******************************************************************
 *               Funciones auxiliares                              *
 *******************************************************************/

void free_nombres(char **nombres, int count)
{
	for (int i = 0; i < count; i++) {
		free(nombres[i]);
	}
	free(nombres);
}
char *obtener_al_azal(char **nombres, int count)
{
	int random = rand() % count;
	return nombres[random];
}

char *copiar_nombre_pokemon(const char *string_a_copiar)
{
	if (!string_a_copiar)
		return NULL;

	char *string_copiado = malloc(strlen(string_a_copiar) + 1);
	if (!string_copiado)
		return NULL;

	strcpy(string_copiado, string_a_copiar);
	return string_copiado;
}
void extraer_nombres(char *csv_pokemones, char **nombres,
		     int *cant_poke_counter)
{
	char *token;
	*cant_poke_counter = 0;
	token = strtok(csv_pokemones, ",");
	while (token != NULL && *cant_poke_counter < MAX_NOMBRES) {
		nombres[*cant_poke_counter] = copiar_nombre_pokemon(token);
		if (nombres[*cant_poke_counter] == NULL) {
			printf("Error: Memory allocation failed.\n");
			free_nombres(nombres, *cant_poke_counter);
			return;
		}
		(*cant_poke_counter)++;
		token = strtok(NULL, ",");
	}
}
char *obtener_fuerza(struct TP *tp, char *nombre, int jugador)
{
	const struct pokemon_info *pokemon =
		tp_pokemon_seleccionado(tp, jugador);
	if (pokemon == NULL) {
		return NULL;
	}
	char *fuerza = malloc(10);
	if (fuerza == NULL) {
		return NULL;
	}
	sprintf(fuerza, "%d", pokemon->fuerza);
	return fuerza;
}
char *obtener_destreza(struct TP *tp, char *nombre, int jugador)
{
	const struct pokemon_info *pokemon =
		tp_pokemon_seleccionado(tp, jugador);
	if (pokemon == NULL) {
		return NULL;
	}
	char *destreza = malloc(10);
	if (destreza == NULL) {
		return NULL;
	}
	sprintf(destreza, "%d", pokemon->destreza);
	return destreza;
}
char *obtener_inteligencia(struct TP *tp, char *nombre, int jugador)
{
	const struct pokemon_info *pokemon =
		tp_pokemon_seleccionado(tp, jugador);
	if (pokemon == NULL) {
		return NULL;
	}
	char *inteligencia = malloc(10);
	if (inteligencia == NULL) {
		return NULL;
	}
	sprintf(inteligencia, "%d", pokemon->inteligencia);
	return inteligencia;
}
char *obtener_fuerza_pokemon_no_seleccionado(struct TP *tp, char *nombre)
{
	const struct pokemon_info *pokemon = tp_buscar_pokemon(tp, nombre);
	if (pokemon == NULL) {
		return NULL;
	}
	char *fuerza = malloc(10);
	if (fuerza == NULL) {
		return NULL;
	}
	sprintf(fuerza, "%d", pokemon->fuerza);
	return fuerza;
}
char *obtener_destreza_pokemon_no_seleccionado(struct TP *tp, char *nombre)
{
	const struct pokemon_info *pokemon = tp_buscar_pokemon(tp, nombre);
	if (pokemon == NULL) {
		return NULL;
	}
	char *destreza = malloc(10);
	if (destreza == NULL) {
		return NULL;
	}
	sprintf(destreza, "%d", pokemon->destreza);
	return destreza;
}
char *obtener_inteligencia_pokemon_no_seleccionado(struct TP *tp, char *nombre)
{
	const struct pokemon_info *pokemon = tp_buscar_pokemon(tp, nombre);
	if (pokemon == NULL) {
		return NULL;
	}
	char *inteligencia = malloc(10);
	if (inteligencia == NULL) {
		return NULL;
	}
	sprintf(inteligencia, "%d", pokemon->inteligencia);
	return inteligencia;
}
/*******************************************************************
 *               Funciones Principales                             *
 *******************************************************************/
menu_t *menu_crear()
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (menu == NULL) {
		return NULL;
	}
	menu->dificultad = FACIL;
	return menu;
}
void seleccionar_jugada(struct TP *tp, menu_t *menu)
{
	printf("\033[H\033[J");
	/* 	if (menu->rondas_jugadas == 0)
	{
		//elegir_dificultad(menu);
	} */
	char *pokemon_elegido = NULL;
	if (menu->rondas_jugadas == 0) {
		pokemon_elegido = seleccionar_pokemon(tp);
	} else {
		pokemon_elegido =
			copiar_nombre_pokemon(tp->jugador1->pokemon->nombre);
	}
	if (pokemon_elegido == NULL) {
		printf("Error al elegir el pokemon.\n");
		return;
	}
	char *pokemon_elegido_al_azar = NULL;
	if (menu->rondas_jugadas == 0) {
		pokemon_elegido_al_azar = seleccionar_pokemon_al_azar(tp);
	} else {
		pokemon_elegido_al_azar =
			copiar_nombre_pokemon(tp->jugador2->pokemon->nombre);
	}

	if (pokemon_elegido_al_azar == NULL) {
		printf("Error al elegir el pokemon.\n");
		return;
	}
	printf("\033[H\033[J");
	tp_seleccionar_pokemon(tp, JUGADOR1, pokemon_elegido);
	tp_seleccionar_pokemon(tp, JUGADOR2, pokemon_elegido_al_azar);
	char *fuerza1 = obtener_fuerza(tp, pokemon_elegido, JUGADOR1);
	char *fuerza2 = obtener_fuerza(tp, pokemon_elegido_al_azar, JUGADOR2);
	char *destreza1 = obtener_destreza(tp, pokemon_elegido, JUGADOR1);
	char *destreza2 =
		obtener_destreza(tp, pokemon_elegido_al_azar, JUGADOR2);
	char *inteligencia1 =
		obtener_inteligencia(tp, pokemon_elegido, JUGADOR1);
	char *inteligencia2 =
		obtener_inteligencia(tp, pokemon_elegido_al_azar, JUGADOR2);
	versus(pokemon_elegido, pokemon_elegido_al_azar, fuerza1, fuerza2,
	       destreza1, destreza2, inteligencia1, inteligencia2);
	sleep(2);
	if (menu->rondas_jugadas == 0) {
		crear_pista(tp, menu);
	} else {
		printf("Desea crear una pista nueva? (S/N)");
		char opti;
		if (scanf(" %c", &opti) != 1) {
			printf("Error al leer la entrada.\n");
			return;
		}
		if (opti == 'S') {
			printf("\033[H\033[J");
			tp_limpiar_pista(tp, JUGADOR1);
			crear_pista(tp, menu);
		} else {
			printf("\033[H\033[J");
			char *ps = tp_obstaculos_pista(tp, JUGADOR1);
			pistas(menu->dificultad, ps, false);
			free(ps);
		}
	}
	if (menu->rondas_jugadas == 0) {
		menu->pista_al_azar = crear_pista_al_azar(tp, menu);
	} else {
		pistas(menu->dificultad, menu->pista_al_azar, false);
	}

	printf("Desea comenzar la carrera? (S/N)");
	char opcion;
	if (scanf(" %c", &opcion) != 1) {
		printf("Error al leer la entrada.\n");
		return;
	}
	if (opcion == 'S') {
		printf("\033[H\033[J");
		mostrar_puntaje_jugador(tp, menu);
	} else if (opcion == 'N') {
		printf("\033[H\033[J");
		//mostrar_menu_principal(tp, menu);
	} else {
		printf("Opción inválida.\n");
		return;
	}
	free(fuerza1);
	free(fuerza2);
	free(destreza1);
	free(destreza2);
	free(inteligencia1);
	free(inteligencia2);
	free(pokemon_elegido);
	free(pokemon_elegido_al_azar);
}
int mostrar_menu_principal(struct TP *tp, menu_t *menu, char opcion)
{
	switch (opcion) {
	case '1':

		return JUGAR;
	case '2':

		return SALIR;
	default:

		return INCORRECTO;
	}
}

bool elegir_dificultad(menu_t *menu, char opcion, int cant_rondas_facil,
		       int cant_rondas_dificil, int cant_rondas_imposible,
		       int *rondas_por_jugar)
{
	bool se_logro = true;
	if (opcion == '1') {
		menu->dificultad = FACIL;
		rondas_por_jugar = cant_rondas_facil;
	} else if (opcion == '2') {
		menu->dificultad = DIFICIL;
		rondas_por_jugar = cant_rondas_dificil;
	} else if (opcion == '3') {
		menu->dificultad = IMPOSIBLE;
		rondas_por_jugar = cant_rondas_imposible;
	} else {
		se_logro = false;
	}
	return se_logro;
}

char *seleccionar_pokemon_al_azar(struct TP *tp)
{
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);
	char *nombre = tp_nombres_disponibles(tp);
	char **nombres = malloc(MAX_NOMBRES * sizeof(char *));
	if (nombres == NULL) {
		printf("Error: Memory allocation failed.\n");
		free(nombre);
		return NULL;
	}
	int cant_poke_counter;

	extraer_nombres(nombre, nombres, &cant_poke_counter);
	char *selected_pokemon = copiar_nombre_pokemon(
		obtener_al_azal(nombres, cant_poke_counter));
	tp_seleccionar_pokemon(tp, JUGADOR2, selected_pokemon);
	free(nombre);
	free_nombres(nombres, cant_poke_counter);
	return selected_pokemon;
}

char *seleccionar_pokemon(struct TP *tp)
{
	char *nombre = tp_nombres_disponibles(struct tp);
	char **nombres = malloc(MAX_NOMBRES * sizeof(char *));
	if (nombres == NULL) {
		printf("Error: Memory allocation failed.\n");
		free(nombre);
		return NULL;
	}
	int cant_poke_counter;

	extraer_nombres(nombre, nombres, &cant_poke_counter);

	for (int i = 0; i < cant_poke_counter; i++) {
		printf("\033[H\033[J");
		char *fuerza =
			obtener_fuerza_pokemon_no_seleccionado(tp, nombres[i]);
		if (fuerza == NULL) {
			printf("Error: Memory allocation failed.\n");
			free(nombre);
			free_nombres(nombres, cant_poke_counter);
			return NULL;
		}
		char *destreza = obtener_destreza_pokemon_no_seleccionado(
			tp, nombres[i]);
		if (destreza == NULL) {
			printf("Error: Memory allocation failed.\n");
			free(nombre);
			free_nombres(nombres, cant_poke_counter);
			return NULL;
		}
		char *inteligencia =
			obtener_inteligencia_pokemon_no_seleccionado(
				tp, nombres[i]);
		if (inteligencia == NULL) {
			printf("Error: Memory allocation failed.\n");
			free(nombre);
			free_nombres(nombres, cant_poke_counter);
			return NULL;
		}
		imprimir_poke_tabla(i, nombres[i], fuerza, inteligencia,
				    destreza);
		if (i == 0) {
			printf(HYEL
			       "1- Elegir este pokemon | 2- Salir | 3- Siguiente\n" RESET);
		} else if (i == cant_poke_counter - 1) {
			printf(HYEL
			       "1- Elegir este pokemon | 2- Salir | 4- Volver al anterior\n" RESET);
		} else {
			printf(HYEL
			       "1- Elegir este pokemon | 2- Salir | 3- Siguiente | 4- Volver al anterior\n" RESET);
		}
		printf(HYEL "Ingrese la opción elegida -> " RESET);
		char opcion;
		if (scanf(" %c", &opcion) != 1) {
			printf("Error al leer la entrada.\n");
			free(fuerza);
			free(destreza);
			free(inteligencia);
			free(nombre);
			free_nombres(nombres, cant_poke_counter);
			return NULL;
		}
		if (opcion == '1') {
			char *selected_pokemon =
				copiar_nombre_pokemon(nombres[i]);
			tp_seleccionar_pokemon(tp, JUGADOR1, selected_pokemon);
			free(nombre);
			free_nombres(nombres, cant_poke_counter);
			free(fuerza);
			free(destreza);
			free(inteligencia);
			return selected_pokemon;
		} else if (opcion == '2') {
			salida();
			free(nombre);
			free_nombres(nombres, cant_poke_counter);
			free(fuerza);
			free(destreza);
			free(inteligencia);
			return NULL;
		} else if (opcion == '4') {
			i -= 2;
		}
		free(fuerza);
		free(destreza);
		free(inteligencia);
	}
	free(nombre);
	free_nombres(nombres, cant_poke_counter);
	return NULL;
}

char *crear_pista_al_azar(struct TP *tp, menu_t *menu)
{
	int cantidad_obstaculos = 0;
	switch (menu->dificultad) {
	case FACIL:
		cantidad_obstaculos = 3;
		break;
	case DIFICIL:
		cantidad_obstaculos = 5;
		break;
	case IMPOSIBLE:
		cantidad_obstaculos = 7;
		break;
	default:
		printf("Dificultad desconocida.\n");
		return NULL;
	}
	char *cadena = malloc((size_t)(cantidad_obstaculos + 1) * sizeof(char));
	if (cadena == NULL) {
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}

	const char opciones[] = { 'F', 'D', 'I' };
	for (int i = 0; i < cantidad_obstaculos; i++) {
		unsigned int seed = (unsigned int)time(NULL);
		srand(seed);
		int indice = rand() % 3;
		cadena[i] = opciones[indice];
		int obstaculo = 0;
		if (cadena[i] == 'D') {
			obstaculo = OBSTACULO_DESTREZA;
		} else if (cadena[i] == 'I') {
			obstaculo = OBSTACULO_INTELIGENCIA;
		}
		tp_agregar_obstaculo(tp, JUGADOR2, obstaculo, (unsigned int)i);
	}
	int posicion_X = rand() % cantidad_obstaculos;
	switch (menu->dificultad) {
	case FACIL:
		cadena[posicion_X] = 'X';
		break;
	case DIFICIL:
		cadena[posicion_X] = 'X';
		posicion_X = rand() % cantidad_obstaculos;
		cadena[posicion_X] = 'X';
		break;
	case IMPOSIBLE:
		for (int i = 0; i < 7; i++) {
			cadena[rand() % cantidad_obstaculos] = 'X';
		}
		break;
	}
	cadena[cantidad_obstaculos] = '\0';
	pistas(menu->dificultad, cadena, false);
	return cadena;
}

void crear_pista(struct TP *tp, menu_t *menu)
{
	printf("\033[H\033[J");
	int cantidad_obstaculos = 0;

	switch (menu->dificultad) {
	case FACIL:
		cantidad_obstaculos = 3;
		pistas(FACIL, "XXX", true);
		break;
	case DIFICIL:
		cantidad_obstaculos = 5;
		pistas(DIFICIL, "XXXXX", true);
		break;
	case IMPOSIBLE:
		cantidad_obstaculos = 7;
		pistas(IMPOSIBLE, "XXXXXXX", true);
		break;
	default:
		printf("Dificultad desconocida.\n");
		return;
	}

	for (int i = 0; i < cantidad_obstaculos; i++) {
		printf("                ┌───────────────────────────────────────────────────────────────────────────────────────┐\n");
		printf("                │        (F) Fuerza (D) Destreza (I) Inteligencia " HRED
		       "(1) Salir  " RESET "             │\n");
		printf("                └───────────────────────────────────────────────────────────────────────────────────────┘\n");
		printf(HYEL "Ingrese la opción que desea ingresar -> " RESET);
		char opcion;
		if (scanf(" %c", &opcion) != 1) {
			printf("Error al leer la entrada.\n");
			return;
		}
		printf("\033[H\033[J");
		char *pista_actual = NULL;
		char pista_completa[cantidad_obstaculos + 1];

		switch (opcion) {
		case 'F':
			tp_agregar_obstaculo(tp, JUGADOR1, OBSTACULO_FUERZA,
					     (unsigned int)i);
			break;
		case 'D':
			tp_agregar_obstaculo(tp, JUGADOR1, OBSTACULO_DESTREZA,
					     (unsigned int)i);
			break;
		case 'I':
			tp_agregar_obstaculo(tp, JUGADOR1,
					     OBSTACULO_INTELIGENCIA,
					     (unsigned int)i);
			break;
		case '1':
			tp_limpiar_pista(tp, JUGADOR1);
			salida();
			return;
		default:

			printf("Opción inválida.\n");
			i--;
			continue;
		}

		pista_actual = tp_obstaculos_pista(tp, JUGADOR1);
		if (pista_actual == NULL) {
			pista_actual = copiar_nombre_pokemon("X");
			printf("Error al obtener la pista.\n");
		}

		int len_origen = (int)strlen(pista_actual);
		for (int j = 0; j < cantidad_obstaculos; j++) {
			if (j < len_origen) {
				pista_completa[j] = pista_actual[j];
			} else {
				pista_completa[j] = 'X';
			}
		}
		pista_completa[cantidad_obstaculos] = '\0';

		pistas(menu->dificultad, pista_completa, true);
		free(pista_actual);
	}

	return;
}

int cantidad_de_rondas_jugadas(menu_t *menu)
{
	return menu->cantidad_de_rondas_por_jugar;
}

bool finalizo_carrera(menu_t *menu)
{
	return menu->cantidad_de_rondas_por_jugar == 0;
}

void porcentaje_por_rondas(menu_t *menu)
{
	char str[10], str2[10], str3[10];
	sprintf(str, "%.1f", menu->puntaje_1ronda);
	sprintf(str2, "%.1f", menu->puntaje_2ronda);
	sprintf(str3, "%.1f", menu->puntaje_3ronda);
	if (menu->rondas_jugadas == 1) {
		puntajes_por_ronda(str, "-", "-");
	} else if (menu->rondas_jugadas == 2) {
		puntajes_por_ronda(str, str2, "-");
	} else if (menu->rondas_jugadas == 3) {
		puntajes_por_ronda(str, str2, str3);
	}
	salida();
}

void mostrar_puntaje_jugador(struct TP *tp, menu_t *menu)
{
	printf("\033[H\033[J");
	unsigned int tiempo1 = tp_calcular_tiempo_pista(tp, JUGADOR1);
	unsigned int tiempo2 = tp_calcular_tiempo_pista(tp, JUGADOR2);
	int resta = (int)(tiempo1 - tiempo2);
	if (resta < 0) {
		resta = -resta;
	}
	char str[10];
	float puntaje1 =
		(float)100 - (float)(100 * resta / (int)(tiempo1 + tiempo2));
	sprintf(str, "%.1f", puntaje1);
	bool ganada = false;
	if (menu->dificultad == FACIL && puntaje1 >= 50) {
		ganada = true;
	} else if (menu->dificultad == DIFICIL && puntaje1 >= 70) {
		ganada = true;
	} else if (menu->dificultad == IMPOSIBLE && puntaje1 >= 80) {
		ganada = true;
	} else {
		ganada = false;
	}

	menu->rondas_jugadas++;
	if (menu->rondas_jugadas == 1) {
		menu->puntaje_1ronda = puntaje1;
	} else if (menu->rondas_jugadas == 2) {
		menu->puntaje_2ronda = puntaje1;
	} else if (menu->rondas_jugadas == 3) {
		menu->puntaje_3ronda = puntaje1;
	}
	fin_de_la_partida(ganada, menu->cantidad_de_rondas_por_jugar, str);

	if (menu->cantidad_de_rondas_por_jugar == 0) {
		porcentaje_por_rondas(menu);
	}
	if (menu->cantidad_de_rondas_por_jugar != 0) {
		printf("Desea jugar otra ronda? (S/N)");
		char opcion;
		if (scanf(" %c", &opcion) != 1) {
			printf("Error al leer la entrada.\n");
			return;
		}
		if (opcion == 'S') {
			printf("\033[H\033[J");
			menu->cantidad_de_rondas_por_jugar--;
			seleccionar_jugada(tp, menu);
		} else if (opcion == 'N') {
			printf("\033[H\033[J");
			porcentaje_por_rondas(menu);
		} else {
			printf("Opción inválida.\n");
			return;
		}
	}
	return;
}

void menu_destruir(menu_t *menu)
{
	free(menu);
	return;
}