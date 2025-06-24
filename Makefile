# Compilador
CC = gcc

# Carpetas
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib

# Flags
CFLAGS = -I$(INCLUDE_DIR) -Wno-unused-result
LDFLAGS = -L$(LIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm

# Archivos fuente (código principal + tdas)
SRC_FILES = $(SRC_DIR)/poker.c \
            $(wildcard $(SRC_DIR)/tdasPoker/*.c) \
            $(wildcard $(SRC_DIR)/tdas/*.c)

# Ejecutable
OUT = $(BUILD_DIR)/poker.exe

# Regla por defecto
all: $(OUT)

# Compilar
$(OUT): $(SRC_FILES)
	@echo 🛠 Compilando el juego de Poker Texas Hold'em...
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SRC_FILES) -o $(OUT) $(CFLAGS) $(LDFLAGS)
	@echo ✅ Compilación exitosa.

# Ejecutar
run: all
	@echo 🚀 Ejecutando el juego...
	./$(OUT)

# Limpiar
clean:
	@echo 🧹 Limpiando ejecutables...
	@if exist $(BUILD_DIR) del /Q $(BUILD_DIR)\*.exe || true
