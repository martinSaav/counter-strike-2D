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

# Crear carpeta de compilación
RUN mkdir -p build

# Configurar y compilar solo el servidor
RUN cmake --build build/ --target taller_server

# Exponer el puerto del servidor
EXPOSE 8080

# El contenedor no hace nada al correr
CMD ["bash"]
