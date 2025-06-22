FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Instalar solo las herramientas necesarias para compilar
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    git \
    gcc \
    g++ \
    build-essential \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Directorio de trabajo
WORKDIR /app

# Copiar el código al contenedor
COPY . .

RUN rm -rf build && mkdir build

RUN cmake -S . -B build

RUN cmake --build build --target taller_server

EXPOSE 50000

CMD ["./build/taller_server", "50000", "/app/server/server_config.yaml"]
