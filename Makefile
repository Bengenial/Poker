# Makefile para compilar el juego de Poker Texas Hold'em

# Variables
CC = gcc
SRC = src/poker.c $(wildcard src/juegoPoker/*.c) $(wildcard src/tdas/*.c)
OUT_DIR = build
OUT_EXE = $(OUT_DIR)/poker.exe

# Regla por defecto
all: $(OUT_EXE)

# Crear el ejecutable
$(OUT_EXE): $(SRC)
	@echo "🛠 Compilando el juego de Poker Texas Hold'em..."
	@mkdir -p $(OUT_DIR)
	$(CC) $(SRC) -o $(OUT_EXE)
	@echo "✅ Compilación exitosa."

# Ejecutar el juego
run: all
	@echo "🚀 Ejecutando el juego..."
	@./$(OUT_EXE)

# Limpiar archivos generados
clean:
	@echo "🧹 Limpiando..."
	rm -rf $(OUT_DIR)