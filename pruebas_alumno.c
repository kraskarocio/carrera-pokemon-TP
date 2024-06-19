#include "pa2m.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/tp.h"

void tp_crear_pruebas_archivo_NULL_devuelve_NULL()
{
	TP *tp = tp_crear(NULL);
	pa2m_afirmar(tp == NULL, "TP creado sin archivo es NULL");
}

void tp_crear_pruebas()
{
	const char *nombre_del_archivo = "ejemplo/pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	pa2m_afirmar(
		tp_cantidad_pokemon(tp) == 25,
		"La cantidad de pokemones coincide con la cantidad de lineas del archivo");
	tp_destruir(tp);
}
void tp_bucar_un_nombre_de_pokemon_que_no_esta_devuelve_NULL()
{
	const char *nombre_del_archivo = "ejemplo/pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	pa2m_afirmar(tp_buscar_pokemon(tp, NULL) == NULL,
		     "Si no se ingresa el nombre de un pokemon devuelve NULL.");
	const char *abc = "abc";
	pa2m_afirmar(
		tp_buscar_pokemon(tp, abc) == NULL,
		"Si el nombre no pertenece a ninguno de los pokemones, devuelve NULL.");
	tp_destruir(tp);
}

void tp_buscar_busco_pokemones()
{
	const char *nombre_del_archivo = "ejemplo/pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	const char *poke1_name = "Pikachu";
	const struct pokemon_info *poke1 = tp_buscar_pokemon(tp, poke1_name);
	pa2m_afirmar(poke1 != NULL,
		     "Pokemon no nulo, con mayuscula encontrado.");
	pa2m_afirmar(poke1->fuerza == 10, "Atributo correcto");
	pa2m_afirmar(poke1->destreza == 9, "Atributo correcto");
	pa2m_afirmar(poke1->inteligencia == 8, "Atributo correcto");
	const char *poke2_name = "sandshrew";
	const struct pokemon_info *poke2 = tp_buscar_pokemon(tp, poke2_name);
	pa2m_afirmar(poke2 != NULL,
		     "Pokemon no nulo, pokemon con minuscula encontrado.");
	pa2m_afirmar(poke2->fuerza == 3, "Atributo correcto");
	pa2m_afirmar(poke2->destreza == 7, "Atributo correcto");
	pa2m_afirmar(poke2->inteligencia == 7, "Atributo correcto");
	const char *poke3_name = "dragonite";
	const struct pokemon_info *poke3 = tp_buscar_pokemon(tp, poke3_name);
	pa2m_afirmar(poke3 != NULL,
		     "Pokemon no nulo, pokemon con minuscula encontrado.");
	pa2m_afirmar(poke3->fuerza == 9, "Atributo correcto");
	pa2m_afirmar(poke3->destreza == 10, "Atributo correcto");
	pa2m_afirmar(poke3->inteligencia == 8, "Atributo correcto");
	tp_destruir(tp);
}
void tp_buscar_con_otro_archivo_con_todos_los_pokemones()
{
	const char *nombre_del_archivo = "ejemplo/menos_pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	const char *poke1_name = "ivysaur";
	const char *poke2_name = "Blastoise";
	const char *poke3_name = "wartortle";
	const char *poke4_name = "Raichu";
	const struct pokemon_info *poke1 = tp_buscar_pokemon(tp, poke1_name);
	const struct pokemon_info *poke2 = tp_buscar_pokemon(tp, poke2_name);
	const struct pokemon_info *poke3 = tp_buscar_pokemon(tp, poke3_name);
	const struct pokemon_info *poke4 = tp_buscar_pokemon(tp, poke4_name);
	pa2m_afirmar(poke1 != NULL,
		     "Pokemon 1 no nulo, pokemon con minuscula encontrado.");
	pa2m_afirmar(poke2 != NULL,
		     "Pokemon 2 no nulo, pokemon con mayuscula encontrado.");
	pa2m_afirmar(poke3 != NULL,
		     "Pokemon 3 no nulo, pokemon con minuscula encontrado.");
	pa2m_afirmar(poke4 != NULL,
		     "Pokemon 4 no nulo, pokemon con mayuscula encontrado.");
	pa2m_afirmar(poke1->fuerza == 8, "Pokemon 1. Atributo correcto");
	pa2m_afirmar(poke1->destreza == 7, "Pokemon 1. Atributo correcto");
	pa2m_afirmar(poke1->inteligencia == 7, "Pokemon 1. Atributo correcto");
	pa2m_afirmar(poke2->fuerza == 8, "Pokemon 2. Atributo correcto");
	pa2m_afirmar(poke2->destreza == 8, "Pokemon 2. Atributo correcto");
	pa2m_afirmar(poke2->inteligencia == 9, "Pokemon 2. Atributo correcto");
	pa2m_afirmar(poke3->fuerza == 9, "Pokemon 3. Atributo correcto");
	pa2m_afirmar(poke3->destreza == 7, "Pokemon 3. Atributo correcto");
	pa2m_afirmar(poke3->inteligencia == 7, "Pokemon 3. Atributo correcto");
	pa2m_afirmar(poke4->fuerza == 5, "Pokemon 4. Atributo correcto");
	pa2m_afirmar(poke4->destreza == 8, "Pokemon 4. Atributo correcto");
	pa2m_afirmar(poke4->inteligencia == 7, "Pokemon 4. Atributo correcto");
	tp_destruir(tp);
}
void tp_nombres_disponibles_pruebas_archivo_mas_chico()
{
	const char *nombre_del_archivo = "ejemplo/menos_pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	char *nombres = tp_nombres_disponibles(tp);
	pa2m_afirmar(strcmp(nombres, "Blastoise,Ivysaur,Raichu,Wartortle") == 0,
		     "Nombres correctos, archivo chico.");
	free(nombres);
	tp_destruir(tp);
}
void tp_nombres_disponibles_pruebas_archivo_grande()
{
	const char *nombre_del_archivo = "ejemplo/pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	char *nombres = tp_nombres_disponibles(tp);
	pa2m_afirmar(
		strcmp(nombres,
		       "Articuno,Blastoise,Bulbasaur,Caterpie,Charizard,Diglett,Dragonair,Dragonite,Dratini,Ekans,Geodude,Grimer,Ivysaur,Meowth,Moltres,Pidgey,Pikachu,Raichu,Rattata,Sandshrew,Squirtle,Venusaur,Vulpix,Wartortle,Zapdos") ==
			0,
		"Nombres correctos, archivo grande.");
	free(nombres);
	tp_destruir(tp);
}
void tp_nombres_disponibles_si_jugador_ya_tiene_seleccionado()
{
	const char *nombre_del_archivo = "ejemplo/pokemones.txt";
	TP *tp = tp_crear(nombre_del_archivo);
	bool resta = tp_seleccionar_pokemon(tp, JUGADOR_1, "Pikachu");
	if (resta == false) {
		printf("No se pudo seleccionar el pokemon\n");
	}

	char *nombres = tp_nombres_disponibles(tp);
	pa2m_afirmar(
		strcmp(nombres,
		       "Articuno,Blastoise,Bulbasaur,Caterpie,Charizard,Diglett,Dragonair,Dragonite,Dratini,Ekans,Geodude,Grimer,Ivysaur,Meowth,Moltres,Pidgey,Raichu,Rattata,Sandshrew,Squirtle,Venusaur,Vulpix,Wartortle,Zapdos") ==
			0,
		"Nombres correctos, archivo grande.");
	free(nombres);
	tp_destruir(tp);
}

void tp_seleccionar_pokemon_devuelve_correcto()
{
	TP *tp = tp_crear("ejemplo/pokemones.txt");
	pa2m_afirmar(tp_seleccionar_pokemon(tp, JUGADOR_1, "Pikachu") == true,
		     "Se selecciono correctamente el pokemon");
	pa2m_afirmar(tp_seleccionar_pokemon(tp, JUGADOR_2, "Pikachu") == false,
		     "No se selecciono el mismo pokemon que el jugador 1");
	pa2m_afirmar(tp_seleccionar_pokemon(tp, JUGADOR_2, "Blastoise") == true,
		     "Se selecciono correctamente el pokemon");
	pa2m_afirmar(tp_seleccionar_pokemon(tp, JUGADOR_1, "Blastoise") ==
			     false,
		     "No se selecciono el mismo pokemon que el jugador 2");
	const struct pokemon_info *poke1 =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	const struct pokemon_info *poke2 =
		tp_pokemon_seleccionado(tp, JUGADOR_2);
	pa2m_afirmar(poke1 != NULL, "Pokemon 1 seleccionado no es NULL");
	pa2m_afirmar(poke2 != NULL, "Pokemon 2 seleccionado no es NULL");
	pa2m_afirmar(strcmp(poke1->nombre, "Pikachu") == 0,
		     "Pokemon del jugador 1. Nombre correcto");
	pa2m_afirmar(strcmp(poke2->nombre, "Blastoise") == 0,
		     "Pokemon del jugador 2. Nombre correcto");
	pa2m_afirmar(poke1->fuerza == 10,
		     "Pokemon del jugador 1. Atributo correcto");
	pa2m_afirmar(poke1->destreza == 9,
		     "Pokemon del jugador 1. Atributo correcto");
	pa2m_afirmar(poke1->inteligencia == 8,
		     "Pokemon del jugador 1. Atributo correcto");
	pa2m_afirmar(poke2->fuerza == 8,
		     "Pokemon del jugador 2. Atributo correcto");
	pa2m_afirmar(poke2->destreza == 8,
		     "Pokemon del jugador 2. Atributo correcto");
	pa2m_afirmar(poke2->inteligencia == 9,
		     "Pokemon del jugador 2. Atributo correcto");
	tp_destruir(tp);
}


int main()
{
	pa2m_nuevo_grupo(
		"\n======================== TP CREAR ========================");
	tp_crear_pruebas_archivo_NULL_devuelve_NULL();
	tp_crear_pruebas();
	pa2m_nuevo_grupo(
		"\n======================== TP BUSCAR ========================");
	tp_bucar_un_nombre_de_pokemon_que_no_esta_devuelve_NULL();
	tp_buscar_busco_pokemones();
	tp_buscar_con_otro_archivo_con_todos_los_pokemones();
		pa2m_nuevo_grupo(
		"\n======================== TP_NOMBRE_DISPONIBLES ========================");
	tp_nombres_disponibles_pruebas_archivo_mas_chico();
	tp_nombres_disponibles_pruebas_archivo_grande();
	tp_nombres_disponibles_si_jugador_ya_tiene_seleccionado();
	pa2m_nuevo_grupo(
			"\n========= TP_SELECCIONAR_POKEMON / TP_POKEMON_SELECCIONADO =========");
	tp_seleccionar_pokemon_devuelve_correcto();
	return pa2m_mostrar_reporte();
}
