#include "src/tp.h"
#include "src/menu.h"
#include "src/ANSI-color-codes.h"
#include "src/ASCII_print.h"
#include <stdio.h>
#include <time.h>

typedef struct juego {
	int *cantidad_de_rondas_por_jugar;
	int rondas_jugadas;
	float puntaje_1ronda;
	float puntaje_2ronda;
	float puntaje_3ronda;
	char *pista_al_azar;
};

struct juego *crear_juego()
{
	struct juego *juego = malloc(sizeof(juego_t));
	juego->cantidad_de_rondas_jugadas = NULL;
	juego->rondas_jugadas = 0;
	juego->puntaje_1ronda = 0.0f;
	juego->puntaje_2ronda = 0.0f;
	juego->puntaje_3ronda = 0.0f;
	juego->pista_al_azar = NULL;
}

void eliminar_juego(struct juego *juego)
{
	free(juego->pista_al_azar);
	free(juego);
}

int main(int argc, char const *argv[])
{
	struct TP *tp = tp_crear("ejemplo/pokemones.txt");
	menu_t *menu = menu_crear();
	struct juego *juego = crear_juego();
	char opcion;
	printf("\033[H\033[J");
	imprimir_menu_principal();
	printf(HYEL "Ingrese la opción elegida -> " RESET);

	if (scanf(" %c", &opcion) != 1) {
		printf("Error al leer la entrada.\n");
		return -1;
	}

	int opcion_ingresada_menu_principal =
		mostrar_menu_principal(tp, menu, opcion);
	switch (opcion_ingresada_menu_principal) {
	case 1:
		printf("\033[H\033[J");
		imprimir_dificultades(3, 5, 7, 2, 1, 0, 50, 70, 80, 1, 2, 6);
		printf(HYEL "Ingrese la dificultad -> " RESET);
		char opcion_dificultad;
		if (scanf(" %c", &opcion_dificultad) != 1) {
			printf("Error al leer la entrada.\n");
			return -1;
		}
		bool se_pudo_dif =
			elegir_dificultad(menu, opcion_dificultad, 2, 1, 0,
					  juego->cantidad_de_rondas_por_jugar);
		if (se_pudo_dif == false) {
			main(argc, argv);
		}
		seleccionar_pokemon(tp);
		break;
	case 2:
		printf("\033[H\033[J");
		salida();
		break;
	default:
		printf("\033[H\033[J");
		main(argc, argv);
		break;
	}
	eliminar_juego(juego);
	tp_destruir(tp);
	menu_destruir(menu);
	return 0;
}
