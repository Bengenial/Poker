@echo off
echo 🛠 Compilando el juego de Poker Texas Hold'em...

REM Crear carpeta build si no existe
if not exist build mkdir build

REM Compilar usando los headers y librerías locales
gcc src\poker.c src\tdasPoker\*.c src\tdas\*.c -o build\poker.exe

IF %ERRORLEVEL% NEQ 0 (
    echo ❌ Error en la compilacion.
    pause
    exit /b
)

echo ✅ Compilacion exitosa.

echo 🚀 Ejecutando el juego...
build\poker.exe
