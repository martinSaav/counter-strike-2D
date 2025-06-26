#!/bin/bash

set -e

NAME="cs2d"
SERVER_NAME="taller_server"
CLIENT_NAME="taller_client"
TEST_NAME="taller_tests"


echo "Instalando $NAME..."

echo "Instalando dependencias..."
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  libsdl2-dev \
  libsdl2-image-dev \
  libsdl2-mixer-dev \
  libopus-dev \
  libopusfile-dev \
  libxmp-dev \
  libfluidsynth-dev \
  libwavpack-dev \
  libfreetype-dev \
  qt6-base-dev \
  qt6-tools-dev \
  qt6-tools-dev-tools

echo "Compilando el proyecto..."
make compile-debug

echo "Corriendo tests unitarios..."

./build/$TEST_NAME

echo "Instalando archivos..."

# Crear directorios de destino
sudo mkdir -p /usr/bin
sudo mkdir -p /var/$NAME/
sudo mkdir -p /etc/$NAME/

# Copiar binarios
sudo cp build/$SERVER_NAME /usr/bin/
sudo cp build/$CLIENT_NAME /usr/bin/


# Copiar assets
sudo cp -r client/data /var/$NAME/

# Copiar configuración 
sudo cp -r server/server_config.yaml /etc/$NAME/

echo "🎉 Instalación completada correctamente."
