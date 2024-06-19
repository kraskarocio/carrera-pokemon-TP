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
	return pa2m_mostrar_reporte();
}
