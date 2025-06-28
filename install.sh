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

SERVER_EXECUTABLE="server.sh"
CLIENT_EXECUTABLE="client.sh"

CONFIG_DIR="/etc/$NAME"
ASSETS_DIR="/var/$NAME"
BIN_DIR="/usr/bin"
DESKTOP_DIR="$HOME/Desktop"
RELEASE_URL="https://github.com/martinSaav/counter-strike-2D/archive/refs/tags/1.zip"

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
  wget \
  libsdl2-dev \
  libsdl2-image-dev \
  libsdl2-mixer-dev \
  libopus-dev \
  libopusfile-dev \
  libxmp-dev \
  libfluidsynth-dev \
  fluidsynth \
  libwavpack1 \
  libwavpack-dev \
  wavpack \
  libfreetype-dev \
  qt6-base-dev \
  qt6-tools-dev \
  qt6-tools-dev-tools \
  libyaml-cpp-dev


# ------------------------ Descargar release ------------------------
echo -e "${BLUE}📥 Descargando la última release...${NC}"
TMP_DIR=$(mktemp -d)
wget -q -O "$TMP_DIR/release.zip" "$RELEASE_URL"
unzip -q "$TMP_DIR/release.zip" -d "$TMP_DIR"
PROJECT_DIR=$(find "$TMP_DIR" -type d -name "counter-strike-2D-*")
cd "$PROJECT_DIR"

# ------------------------ Compilacion ------------------------
echo -e "${BLUE}🧱 Compilando el proyecto...${NC}"
make compile-debug

# ------------------------ Tests ------------------------
echo -e "${BLUE}🧪 Corriendo tests unitarios...${NC}"
./build/$TEST_NAME

# ------------------------ Instalacion ------------------------
echo -e "${BLUE}📁 Instalando archivos del juego...${NC}"
sudo mkdir -p "$BIN_DIR" "$ASSETS_DIR" "$CONFIG_DIR"
sudo cp build/$SERVER_NAME "$BIN_DIR/"
sudo cp build/$CLIENT_NAME "$BIN_DIR/"

# Assets
sudo cp -r client/data/* "$ASSETS_DIR/"

# Dar permisos a los assets
sudo chmod -R 777 "$ASSETS_DIR"

# Configuracion
sudo cp server/server_config.yaml "$CONFIG_DIR/"

# ------------------------ Scripts del servidor y cliente ------------------------
echo -e "${BLUE}📄 Creando scripts de arranque...${NC}"

cat <<EOF > "$TMP_DIR/$SERVER_EXECUTABLE"
#!/bin/bash
cd $BIN_DIR
PORT=8080
CONFIG_PATH="$CONFIG_DIR/server_config.yaml"
./$SERVER_NAME \$PORT \$CONFIG_PATH
EOF

cat <<EOF > "$TMP_DIR/$CLIENT_EXECUTABLE"
#!/bin/bash
cd $BIN_DIR
PORT=8080
#PORT=6000
HOST="localhost"
#HOST="56.124.17.90"
export CS2D_ASSETS_DIR="/var/cs2d"
./$CLIENT_NAME \$HOST \$PORT
EOF

chmod +x "$TMP_DIR/$SERVER_EXECUTABLE" "$TMP_DIR/$CLIENT_EXECUTABLE"
rm -f "$DESKTOP_DIR/$SERVER_EXECUTABLE" "$DESKTOP_DIR/$CLIENT_EXECUTABLE"
cp "$TMP_DIR/$SERVER_EXECUTABLE" "$DESKTOP_DIR/"
cp "$TMP_DIR/$CLIENT_EXECUTABLE" "$DESKTOP_DIR/"

# ------------------------ Finalizacion ------------------------
echo -e "${GREEN}🎉 Instalacion completada con exito.${NC}"
echo -e "${GREEN}📌 Ejecutables en:${NC} $BIN_DIR"
echo -e "${GREEN}📌 Archivos de configuracion en:${NC} $CONFIG_DIR"
echo -e "${GREEN}📌 Datos del juego en:${NC} $ASSETS_DIR"
echo -e "${GREEN}📌 Accesos directos creados en:${NC} $DESKTOP_DIR"
