# Carpetas
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib

# Flags
CFLAGS = -I$(INCLUDE_DIR) -Wno-unused-result
LDFLAGS = -L$(LIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm

# Archivos fuente
SRC_FILES = src/poker.c \
            $(wildcard $(SRC_DIR)/tdasPoker/*.c) \
            $(wildcard $(SRC_DIR)/tdas/*.c)

# Nombre del ejecutable
OUT = $(BUILD_DIR)/poker.exe

# Regla por defecto
all:
	$(CC) $(SRC_FILES) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Ejecutar el juego
run: all
	./$(OUT)

# Limpiar ejecutable
clean:
	del /Q $(BUILD_DIR)\*.exe