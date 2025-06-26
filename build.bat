@echo off
chcp 65001 > nul
echo 🛠 Compilando el juego de Poker Texas Hold'em...

REM Crear carpeta build si no existe
if not exist build mkdir build

REM Compilar usando los headers y librerías locales
gcc src\poker.c src\juegoPoker\*.c src\tdas\*.c -o build\poker.exe

IF %ERRORLEVEL% NEQ 0 (
    echo ❌ Error en la compilacion.
    pause
    exit /b
)

echo ✅ Compilacion exitosa.

echo 🚀 Ejecutando el juego...
start build\poker.exe
