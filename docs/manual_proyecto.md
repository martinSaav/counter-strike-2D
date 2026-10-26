# Manual del proyecto: Counter-Strike 2D

## 1. Objetivo

Desarrollar en C++ un juego multijugador estilo Counter-Strike 2D con arquitectura cliente-servidor, más un editor gráfico de niveles: tres aplicaciones (cliente, servidor y editor). Dos equipos, terroristas y antiterroristas, se enfrentan a lo largo de varias rondas. Cada ronda se gana eliminando al equipo rival, haciendo explotar la bomba (terroristas) o desactivándola (antiterroristas). Gana la partida quien se lleve la mayoría de las rondas.

## 2. Equipo y roles

| Integrante | Rol principal |
|---|---|
| Manuel Herrera Esteban | **Cliente**: render con SDL2, pantallas de Qt, sonido y entrada del usuario. |
| Carlos Matías Sagastume | **Servidor**: lobby, partidas, lógica de juego (rondas, armas, bomba, colisiones). |
| Martín Alejandro Estrada Saavedra | **Protocolo y tests**: mensajes binarios cliente-servidor, sus tests y apoyo general en cliente y servidor. |

## 3. Herramientas

- **C++20** con CMake. SDL2 y libSDL2pp para el juego, Qt6 para el lobby y el editor de niveles, y yaml-cpp para la configuración y los mapas.
- **GoogleTest** para los tests del protocolo, de las reglas del juego y del modelo del editor.
- **GitHub**: una rama por feature y pull requests hacia `dev`, y de `dev` a `main` para las entregas.
- **CI en GitHub Actions**: `pre-commit` (clang-format, cpplint y cppcheck) en cada PR, y compilación con tests en los PR hacia `main`.
- **Valgrind** para verificar memoria y cierre ordenado de los hilos.

## 4. Cronología

| Período | Trabajo |
|---|---|
| Semana 1 (mediados de mayo de 2025) | División de tareas, arquitectura general de hilos y colas, primer protocolo (login, crear y unirse a partidas). |
| Semanas 2 y 3 | Servidor con lobby y partidas, cliente conectándose, movimiento y render del mapa. Los primeros días de junio fueron los de más commits del proyecto. |
| Semanas 4 y 5 | Armas, disparos, colisiones, bomba, compra, rondas, sonidos y pantallas de Qt. Configuración por YAML. |
| Semana 6 (fines de junio de 2025) | Integración, corrección de bugs (rondas, bomba, deadlocks), instalador y documentación. Entrega. |
| Post entrega | Arreglo de la desincronización inicial, código unificado en inglés, correcciones de la devolución (sección 6) y editor de niveles. |

## 5. Problemas que encontramos y cómo los resolvimos

### Deadlocks entre partida y clientes

Al principio la partida y los hilos de cada cliente compartían estado con varios mutex, y en algunos órdenes de bloqueo se trababan. Lo resolvimos centralizando todo en el hilo de `Match`: los clientes solo empujan **comandos** a una cola, la partida los procesa en su loop y publica un snapshot por cola a cada `Sender`. Ningún hilo de cliente toca el estado del juego directamente.

### Lag al comienzo de la partida

El servidor manda 30 estados por segundo desde que arranca la partida, pero el cliente tarda unos segundos en cargar texturas y sonidos. Esos estados se acumulaban y el cliente consumía uno por frame, así que **nunca recuperaba el atraso** y la partida se veía con demora. La solución fue vaciar la cola en cada frame y dibujar solo el estado más nuevo. Además movimos la lectura de teclado y mouse al hilo principal (SDL pide procesar los eventos en el hilo que creó la ventana). Con la medición que dejamos en el cliente (`CS2D_DEBUG_FRAMES=1`) el frame tarda menos de 1 ms y se procesa un estado por frame.

### Cierres inesperados al desconectarse

Si el servidor cerraba la conexión durante una partida, `Protocol` leía un encabezado vacío, lo interpretaba como un tipo de mensaje desconocido y tiraba una excepción que nadie atrapaba en el hilo receptor: el cliente abortaba. Agregamos una excepción específica (`ConnectionClosed`) que los hilos atrapan para terminar de forma ordenada.

### Cambio de bando que nunca ocurría

La ronda del cambio de bando estaba fija en 6 dentro del código, mientras que la configuración tenía 4 rondas: el cambio nunca pasaba. Ahora se calcula a partir de `number_of_rounds` (mitad + 1), igual que la cantidad de rondas necesarias para ganar. Lo cubre un test.

### La bomba no se podía desactivar manteniendo F

Mantener F manda un pedido por frame, y cada pedido reiniciaba el momento de inicio de la desactivación: nunca se llegaba al tiempo necesario. Por eso en la demo no se sabía si la bomba no se desactivaba "por lag". Con el click mantenido para plantar pasaba lo mismo. Ahora el tiempo cuenta desde el primer pedido (moverse lo sigue cancelando), y hay tests que simulan mantener la tecla un frame tras otro.

### Se podía plantar la bomba en cualquier lugar

Lo encontramos al probar un mapa armado con el editor. Con la bomba equipada, si el jugador no estaba en una zona de bomba el código no cortaba ahí: seguía al disparo común, que también llamaba a `fire_gun`, y la bomba empezaba a plantarse sin mirar la posición. Ahora la bomba tiene su propio camino y fuera de una zona el "disparo" se descarta. Lo cubre un test.

### Unirse a una partida ya empezada colgaba el cliente

El servidor tiraba `MatchAlreadyStarted` y, como no se atrapaba, cortaba la conexión sin responder. El cliente quedaba esperando una respuesta que nunca llegaba. Ahora el servidor responde que no se pudo unir, el cliente lo avisa y esas partidas ya no aparecen en la lista.

## 6. Correcciones después de la devolución

A partir de la devolución de la cátedra corregimos:

- **Reglas**: 10 rondas con cambio de bando a la mitad, límite de tiempo por ronda (si se agota sin bomba, ganan los antiterroristas), y un reloj que siempre cuenta hacia atrás, en rojo cuando la bomba está plantada.
- **Balance**: la glock hace más daño que el cuchillo y la AK menos por bala que la glock. Más tiempo de compra, bomba más corta y plata inicial suficiente para comprar un arma.
- **Lobby**: no se puede arrancar sin un jugador por equipo (con confirmación previa) y unirse a una partida empezada ya no cuelga.
- **Armas**: al comprar, la principal vieja queda en el piso. La tecla G tira el arma o la bomba. El M3 muestra su cono de perdigones y no atraviesa paredes.
- **Jugabilidad**: los otros jugadores giran según hacia dónde apuntan. El campo de visión coincide con lo que se dibuja (antes había jugadores que desaparecían dentro del cono). Hay barra de progreso al plantar y desactivar, tienda usable con el mouse, ayuda de controles con H, F11 para alternar ventana y pantalla completa, y más zoom.
- **Sonido**: sonidos de muerte, pasos y desactivación, y volumen según la distancia.
- **Escenarios**: además del desierto, pueblo azteca y zona de entrenamiento, elegibles al crear la partida, cada uno con dos zonas de bomba.
- **Pantallas Qt**: textos que ya no se cortan, un input de usuario legible y la columna K-D explicada.
- **Editor de niveles**: la tercera aplicación que pedía el enunciado. Cada mapa pasó a ser un archivo YAML propio que el editor abre, valida y guarda.

## 7. Qué aprendimos

- **Diseñar la concurrencia antes de escribir código.** El esquema "hilo dueño del estado + colas de comandos y de snapshots" nos sacó de los deadlocks. Cuanto antes lo hubiéramos fijado, menos código habríamos rehecho.
- **Un protocolo binario necesita tests.** Cada campo nuevo implica tocar serialización, deserialización y tamaño en dos lados. Los tests de ida y vuelta nos ahorraron muchos errores de offsets.
- **Probar con el juego corriendo, no solo con tests.** Bugs como el lag inicial, los jugadores invisibles o el cambio de bando que nunca ocurría solo aparecían jugando varias rondas con más de un cliente.
- **Los valores fijos en el código traicionan.** Toda regla que dependa de un número tiene que salir de la configuración.
- **La experiencia de juego también es parte del trabajo.** Un menú que no se entiende o un reloj confuso arruinan una lógica que funciona bien.

## 8. Experiencias personales

> *Borrador: cada integrante completa su parte con sus propias palabras.*

### Manuel Herrera Esteban

- ¿Qué fue lo más difícil del cliente (render, cámara, cono de visión, Qt)?
- ¿Qué harías distinto si lo empezaras de nuevo?
- ¿Qué te llevás del proyecto?

### Carlos Matías Sagastume

- ¿Qué fue lo más difícil de la lógica del servidor (rondas, armas, colisiones, concurrencia)?
- ¿Qué harías distinto si lo empezaras de nuevo?
- ¿Qué te llevás del proyecto?

### Martín Alejandro Estrada Saavedra

- ¿Qué fue lo más difícil del protocolo y de la integración cliente-servidor?
- ¿Qué harías distinto si lo empezaras de nuevo?
- ¿Qué te llevás del proyecto?

## 9. Mejoras a futuro

- Deshacer y rehacer en el editor de niveles, y temas visuales nuevos sin tener que generar texturas.
- Que el servidor tome los mapas nuevos sin reiniciarse.
- Chat de texto o voz durante la partida.
- Cuentas de usuario con estadísticas persistentes.
- Interpolación de movimiento en el cliente para jugar con más latencia.
- Más armas y modos de juego.
