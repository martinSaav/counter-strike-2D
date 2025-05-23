[![language](https://img.shields.io/badge/language-C++-pink.svg?style=flat-square)](https://cplusplus.com/)
![os](https://img.shields.io/badge/OS-linux-blue.svg?style=flat-square)
[![Pre-commit checks](https://github.com/MatiasSagastume/tp-grupal-template/actions/workflows/pre-commit.yml/badge.svg)](https://github.com/MatiasSagastume/tp-grupal-template/actions/workflows/pre-commit.yml)
# Counter-Strike 2D

## Tabla de Contenidos
- [Integrantes](#integrantes)
- [Descripción](#descripción)
- [Requisitos](#requisitos)
- [Instalación](#instalación)
- [Compilación](#compilación)
- [Ejecución](#ejecución)
- [Testing](#testing)
- [Uso de Bibliotecas externas](#uso-de-bibliotecas-externas)
- [Licencia](#licencia)

## Descripción
Este proyecto es un juego de Counter-Strike 2D, desarrollado en C++. El juego permite a los jugadores unirse o crear una partida, elegir su equipo (terroristas o antiterroristas) y jugar.

## Requisitos
- **Sistema Operativo**: Ubuntu 24.04
- Make
- Make
- CMake
- SDL2
- Qt5

**Integrantes**

- [Manuel Herrera Esteban](https://github.com/Manuuherrera)
- [Carlos Marias Sagastume](https://github.com/MatiasSagastume)
- [Martín Alejandro Estrada Saavedra](https://github.com/martinSaav)

## Compilación
Para compilar el proyecto, se debe ejecutar el siguiente comando en la terminal:

```bash
make compile-debug
```

## Ejecución

### Server
```bash
   ./server <port>
```

### Cliente
```bash
   ./client <hostname> <port>
```

## Testing
Para ejecutar los tests, se debe ejecutar el siguiente comando en la terminal:

```bash
   make run-tests
```


## Uso de Bibliotecas externas

1. **Biblioteca de Sockets de la catedra Veiga**
- Autor: eldipa
- Licencia: GPL v2s
- Código fuente: https://github.com/eldipa/sockets-en-cpp

2. **Biblioteca de Threads de la catedra Veiga**
- Autor: eldipa
- Licencia: GPL v2s
- Código fuente: https://github.com/eldipa/hands-on-threads

## Licencia
Este proyecto está bajo la Licencia MIT. Para más detalles, consulta el archivo [LICENSE](LICENSE).
