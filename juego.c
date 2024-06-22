#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "src/tp.h"

#define FACIL 0
#define DIFICIL 1
#define IMPOSIBLE 2

#define JUGADOR_1 0
#define JUGADOR_2 1

#define MAX_NOMBRES 100

typedef struct {
	int dificultad;
	int rondas_por_jugar;
	int obstaculos;
	int puntaje_para_poder_ganar;
} juego_t;

void seleccionar_dificultad(juego_t *juego)
{
	printf("Seleccione la dificultad del juego:\n");
	printf("1. Facil\n");
	printf("2. Dificil\n");
	printf("3. Imposible\n");
	if (scanf("%d", &juego->dificultad) != 1)
		return;
	switch (juego->dificultad) {
	case 1:
		juego->dificultad = FACIL;
		juego->rondas_por_jugar = 3;
		juego->obstaculos = 3;
		juego->puntaje_para_poder_ganar = 60;
		break;
	case 2:
		juego->dificultad = DIFICIL;
		juego->rondas_por_jugar = 2;
		juego->obstaculos = 5;
		juego->puntaje_para_poder_ganar = 70;
		break;
	case 3:
		juego->dificultad = IMPOSIBLE;
		juego->rondas_por_jugar = 1;
		juego->obstaculos = 7;
		juego->puntaje_para_poder_ganar = 80;
		break;
	default:
		break;
	}
}

juego_t *juego_crear()
{
	juego_t *juego = malloc(sizeof(juego_t));
	if (!juego)
		return NULL;
	juego->dificultad = 0;
	juego->rondas_por_jugar = 0;
	juego->obstaculos = 0;
	juego->puntaje_para_poder_ganar = 0;
	return juego;
}

void juego_destruir(juego_t *juego)
{
	free(juego);
}

char *copiar_nombre_pokemon(const char *nombre)
{
	char *nombre_copia = malloc(strlen(nombre) + 1);
	if (nombre_copia == NULL)
		return NULL;
	strcpy(nombre_copia, nombre);
	return nombre_copia;
}

void free_nombres(char **nombres, int cant_poke_counter)
{
	for (int i = 0; i < cant_poke_counter; i++) {
		free(nombres[i]);
	}
	free(nombres);
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
void seleccionar_pokemon(TP *tp)
{
    char *pokemones = tp_nombres_disponibles(tp);
    if (pokemones == NULL) {
        printf("Error: No se pudieron obtener los pokemones disponibles.\n");
        return;
    }

    char **nombres = malloc(MAX_NOMBRES * sizeof(char *));
    if (nombres == NULL) {
        printf("Error: No se pudo reservar memoria.\n");
        free(pokemones);
        return;
    }

    int cant_poke_counter = 0;
    extraer_nombres(pokemones, nombres, &cant_poke_counter);

    printf("Pokemones:\n");
    int i = 0;
    while (i < cant_poke_counter) {
        printf("%d. %s\n", i + 1, nombres[i]);

        if (i == cant_poke_counter - 1) {
            printf("1. Seleccionar | 3. Anterior\n");
        } else if (i == 0) {
            printf("1. Seleccionar | 2. Siguiente\n");
        } else{
			printf("1. Seleccionar | 2. Siguiente | 3. Anterior\n");
		}
        printf("Ingrese el número correspondiente a la opción deseada:\n");

        int rsta;
        if (scanf("%d", &rsta) != 1) {
            printf("Error: Entrada inválida.\n");
            continue;
        }

        if (rsta == 1) {
            char *selected_pokemon = copiar_nombre_pokemon(nombres[i]);
            if (selected_pokemon == NULL) {
                printf("Error: No se pudo copiar el nombre del pokemon.\n");
                free_nombres(nombres, cant_poke_counter); // Free nombres array
                free(pokemones); // Free pokemones string
                return;
            }
            tp_seleccionar_pokemon(tp, JUGADOR_1, selected_pokemon);

            free_nombres(nombres, cant_poke_counter); // Free nombres array
            free(pokemones); // Free pokemones string
            free(selected_pokemon);
            return;
        } else if (rsta == 2) {
            i++;
        } else if (rsta == 3) {
            if (i > 0) {
                i--;
            } else {
                printf("Ya estás en el primer pokemon.\n");
            }
        } else {
            printf("Opción inválida.\n");
        }
    }

    free_nombres(nombres, cant_poke_counter); // Free nombres array
    free(pokemones); // Free pokemones string
    return;
}
void crear_pista_al_azar(TP* tp, int i) {
    int obstaculo = rand() % 3;  // Genera un número aleatorio entre 0 y 2
    tp_agregar_obstaculo(tp, JUGADOR_2, obstaculo, (unsigned)i);
}

void crear_pista(TP* tp, int i){
	printf("ingrese un obstaculo\n");
	printf("1. Fuerza\n");
	printf("2. Destreza\n");
	printf("3. Inteligencia\n");
	int obstaculo;
	if (scanf("%d", &obstaculo) != 1)
		return;
	tp_agregar_obstaculo(tp, JUGADOR_1, obstaculo-1, (unsigned)i);	
}
char *obtener_al_azal(char **nombres, int count)
{
	int random = rand() % count;
	return nombres[random];
}



void seleccionar_pokemon_al_azar(TP *tp)
{
	char *nombre = tp_nombres_disponibles(tp);
	char **nombres = malloc(MAX_NOMBRES * sizeof(char *));
	if (nombres == NULL) {
		printf("Error: Memory allocation failed.\n");
		free(nombre);
		return;
	}
	int cant_poke_counter;

	extraer_nombres(nombre, nombres, &cant_poke_counter);
	char *selected_pokemon = copiar_nombre_pokemon(
		obtener_al_azal(nombres, cant_poke_counter));
	tp_seleccionar_pokemon(tp, JUGADOR_2, selected_pokemon);
	free(nombre);
	free_nombres(nombres, cant_poke_counter);
	free(selected_pokemon);
	return;
}



void tp_jugar_ronda(TP *tp, int ronda, juego_t *juego)
{
	if(ronda == 0){
		seleccionar_pokemon(tp);
	}
	const struct pokemon_info *poke = tp_pokemon_seleccionado(tp, JUGADOR_1);
	printf("Pokemon seleccionado: %s\n", poke->nombre);
	if(ronda == 0){
		seleccionar_pokemon_al_azar(tp);
	}
	const struct pokemon_info *poke2 = tp_pokemon_seleccionado(tp, JUGADOR_2);
	printf("Pokemon seleccionado: %s\n", poke2->nombre);
	if(ronda == 0){
		for(int i = 0; i < juego->obstaculos; i++){
			crear_pista(tp, i);
		}
	} else {
		printf("desea cambiar de pista (s/n)?\n");
		char rsta;
		if (scanf(" %c", &rsta) != 1){
			return;
		}

		if(rsta == 's'){
			tp_limpiar_pista(tp, JUGADOR_1);
			for(int i = 0; i < juego->obstaculos; i++){
				crear_pista(tp, i);
			}
	    }

	}

	char *pista1 = tp_obstaculos_pista(tp, JUGADOR_1);
	printf("pissta: %s\n", pista1);
	free(pista1);
	if(ronda == 0){
		for(int i = 0; i < juego->obstaculos; i++){
			crear_pista_al_azar(tp, i);
		}
	}
	char *pista2 = tp_obstaculos_pista(tp, JUGADOR_2);
	printf("pissta: %s\n", pista2);
	free(pista2);
	unsigned tiempo1 = tp_calcular_tiempo_pista(tp, JUGADOR_1);
	unsigned tiempo2 = tp_calcular_tiempo_pista(tp, JUGADOR_2);
	printf("Tiempo jugador 1: %u\n", tiempo1);
	printf("Tiempo jugador 2: %u\n", tiempo2);

		int resta = (int)(tiempo1 - tiempo2);
	if (resta < 0) {
		resta = -resta;
	}
	float puntaje =
		(float)100 - (float)(100 * resta / (int)(tiempo1 + tiempo2));
	printf("Puntaje: %.2f \n", puntaje);
	if (puntaje >= juego->puntaje_para_poder_ganar) {
		printf("Ganaste!\n");
	} else {
		printf("Perdiste!\n");
	}
}

void tp_jugar(TP *tp, juego_t *juego)
{
	int ronda = 0;
	while (ronda < juego->rondas_por_jugar) {
		tp_jugar_ronda(tp, ronda, juego);
		ronda++;
	}

	
}

int main(int argc, char const *argv[])
{
	srand((unsigned int)time(NULL));

	juego_t *juego = juego_crear();
	if (!juego) {
		printf("Error: No se pudo crear el juego.\n");
		return 1;
	}

	TP *tp = tp_crear("ejemplo/pokemones.txt");
	if (!tp) {
		printf("Error: No se pudo crear TP.\n");
		juego_destruir(juego);
		return 1;
	}

	seleccionar_dificultad(juego);
	tp_jugar(tp, juego);
	juego_destruir(juego);
	tp_destruir(tp);

	return 0;
}
