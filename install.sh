#!/bin/bash


set -e

# ------------------------ Colores ------------------------
GREEN='\033[0;32m'
BLUE='\033[94m'
NC='\033[0m'  # No color

# ------------------------ Variables ------------------------
NAME="cs2d"
NAME_PROJECT="Counter-Strike 2D"
SERVER_NAME="taller_server"
CLIENT_NAME="taller_client"
TEST_NAME="taller_tests"

CONFIG_DIR="/etc/$NAME"
ASSETS_DIR="/var/$NAME"
BIN_DIR="/usr/bin"
DESKTOP_DIR="$HOME/Desktop"

echo -e "${BLUE} ================================== ${NC}"
echo -e "${BLUE} 🚀 Instalador de $NAME_PROJECT${NC}"
echo -e "${BLUE} ================================== ${NC}"

sleep 2

# ------------------------ Dependencias ------------------------
echo -e "${BLUE}📦 Instalando dependencias...${NC}"
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  git \
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
  qt6-tools-dev-tools \
  libyaml-cpp-dev

# ------------------------ Compilacion ------------------------
echo -e "${BLUE}🧱 Compilando el proyecto...${NC}"
make compile-debug

# ------------------------ Tests ------------------------
echo -e "${BLUE}🧪 Corriendo tests unitarios...${NC}"
./build/$TEST_NAME

# ------------------------ Instalacion ------------------------
echo -e "${BLUE}📁 Instalando archivos del juego...${NC}"
sudo mkdir -p "$BIN_DIR"
sudo mkdir -p "$ASSETS_DIR"
sudo mkdir -p "$CONFIG_DIR"

# Binarios
sudo cp build/$SERVER_NAME "$BIN_DIR/"
sudo cp build/$CLIENT_NAME "$BIN_DIR/"

# Assets
sudo cp -r client/data/* "$ASSETS_DIR/"

# Configuracion
sudo cp server/server_config.yaml "$CONFIG_DIR/"

# ------------------------ Scripts del servidor y cliente ------------------------
echo -e "${BLUE}📄 Creando scripts de arranque...${NC}"

# Guardar en raiz del proyecto
PROJECT_DIR=$(pwd)

# Script para levantar el server
cat <<EOF > "$PROJECT_DIR/server.sh"
#!/bin/bash
cd $BIN_DIR
PORT=8080
CONFIG_PATH="$CONFIG_DIR/server_config.yaml"

./$SERVER_NAME \$PORT \$CONFIG_PATH
EOF

# Script para levantar el client
cat <<EOF > "$PROJECT_DIR/client.sh"
#!/bin/bash
cd $BIN_DIR
PORT=8080
HOST="localhost"
export CS2D_ASSETS_DIR="/var/cs2d"
./$CLIENT_NAME \$HOST \$PORT
EOF

# Dar permisos de ejecución
chmod +x "$PROJECT_DIR/server.sh" "$PROJECT_DIR/client.sh"

# Copiar accesos al escritorio
ln -s "$PROJECT_DIR/server.sh" "$DESKTOP_DIR/server.sh"
ln -s "$PROJECT_DIR/client.sh" "$DESKTOP_DIR/client.sh"

# ------------------------ Finalizacion ------------------------

echo -e "${GREEN}🎉 Instalacion completada con exito.${NC}"
echo -e "${GREEN}📌 Ejecutables en:${NC} $BIN_DIR"
echo -e "${GREEN}📌 Archivos de configuracion en:${NC} $CONFIG_DIR"
echo -e "${GREEN}📌 Datos del juego en:${NC} $ASSETS_DIR"
echo -e "${GREEN}📌 Accesos directos creados en:${NC} $DESKTOP_DIR"
