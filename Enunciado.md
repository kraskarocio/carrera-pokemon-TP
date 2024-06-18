# TP: Carrera de obstáculos Pokemon

Se pide implementar un minijuego de carrara de obstáculos Pokemon. El juego debe poder correrse por consola y ser interactivo (el usuario interactúa con el programa mediante entradas de teclado y salidas por pantalla).

La **presentación es parte de la nota**. Se juzga no solo la apariencia (que es importante), si no también la facilidad para interactuar con el juego, la disponibilidad de información que tenga el jugador para poder interactuar, etc.

Se **recomienda fuertemente** la creación de un **TDA** menú para lograr que el código sea los mas prolijo y legible posible.

# Reglas

- Al iniciar el juego, el jugador selecciona la dificultad que desea (facil, normal, difícil, imposible).
- Luego de seleccionada la dificultad se selecciona al azar un Pokemon y se crea una pista de obstáculos.
  - La longitúd de la pista generada depende de la dificultad (a criterio del alumno pero se recomienda que la pista fácil no sea menor a 3 obstáculos)
- Se le presenta al jugador la información del Pokemon contra el cuál va a competir y se le mustra información acerda de la longitud y los obstáculos de la pista
  - Se informa la cantidad y posición de los obstáculos pero se debe omitir información acerca de alguno de los obstáculos.
  - La cantidad de obstáculos omitidos depende de la dificultad (a criterio del alumno)
- Se le pide al jugador que seleccione su propio Pokemon y que arme su propia pista de obstáculos (no hay restricción de tamaño)
- Cuando el jugador termia de armar la pista, se corre una carrera entre ambos pokemon.
- El objetivo del juego es que ambos pokemon terminen la carrera de obstáculos al mismo tiempo o lo mas cercano posible.
- Una vez finalizada la carrera, el jugador puede modificar la pista (no asi cambiar su Pokemon) y volver a correr para mejorar su puntaje.
  - La cantidad de veces que el jugador puede reintentar la carrera depende de la dificultad (a criterio del alumno, pero se recomienda no menos de 2 veces)

# Cálculo del puntaje

El puntaje final de la carrera se calcula como 100-100*|(Ta-Tb)|/(Ta+Tb)

- **Ta** es el tiempo que tarda el primer pokemon en completar la pista
- **Tb** es el tiempo que tarda el segundo pokemon en completar la pista

Ojo con usar enteros que puede quedar todo 0.

# Cálculo del tiempo que se tarda en superar cada obstáculo

Cada Pokemon posee 3 atributos (**fuerza**, **destreza**, **inteligencia**) y a su vez cada uno de los 3 obstáculos posibles corresponden a uno de esos atributos.

El tiempo que tarda cada Pokemon en superar un obstáculo depende de 2 cosas:

- El tipo de obstáculo y el valor del atributo correspondiente del pokemon. La formula utilizada es |10-atributo|
  - Por ejemplo para un pokemon de fuerza 5 y un obstáculo de tipo **FUERZA**, el tiempo que se tarda es |10-5|=5 unidades de tiempo.
- El tipo del obstáculo anterior
  - Si el obstáculo anterior es del mismo tipo, se utiliza 10-**n** de valor base en vez de 10 (donde **n** es la cantidad de obstáculos anteriores seguidos del mismo tipo)
  - En el caso del pokemon anterior, para 2 obstáculos seguidos de fuerza, tarda 5 unidades para el primero pero |10-1-5|=4 para el segundo
  - Si hubiese un tercero, el tiempo sería |10-2-5|=3

Cabe destacar que el tiempo de recorrido de un obstáculo nunca podrá ser menor a 0 unidades de tiempo.

# A entregar
Se debe entregar como mínimo el archivo **tp.h** completo con la implementación pedida, las pruebas que verifiquen el correcto funcionamiento.

También se debe completar el archivo **juego.c** con la lógica principal del juego.

(Supongo que no hay que aclararlo, pero también hay que entregar el informe completo)

# A incluír en el informe

-   Explique la estructura interna del tda **TP**. Explique por qué utilizó esa estructura.
-   Justifique la complejidad de las operaciones implementadas.
