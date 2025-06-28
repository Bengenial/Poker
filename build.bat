@echo off
REM Configura la consola para mostrar caracteres UTF-8 (útil para la salida de GCC)
chcp 65001 > nul

echo Compilando el juego de Poker Texas Hold'em...

REM Crear la carpeta 'build' si no existe.
if not exist build mkdir build

REM Compilar todos los archivos .c necesarios.
gcc src/poker.c src/juegoPoker/*.c src/tdas/*.c -o build/poker.exe

REM Comprobar si hubo un error durante la compilacion.
IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo --- ERROR EN LA COMPILACION ---
    pause
    exit /b 1
)

echo.
echo --- Compilacion exitosa ---
echo.
echo Ejecutando el juego...
start build/poker.exe