#include "../include/raylib.h"
#include "../include/raygui.h"
#include "pantallas.h"
#include "seguridad.h" // Incluimos nuestro nuevo escudo limpio


int main(void) {
    // 1. Quitar los botones de cerrar/minimizar y los bordes de Windows
    SetConfigFlags(FLAG_WINDOW_UNDECORATED); 

    // 2. Iniciar la ventana temporalmente (Raylib necesita que exista para medir el monitor)
    InitWindow(800, 600, "proyecto");
    Texture2D miIcono = LoadTexture("assets/logo.png");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    // 3. Detectar qué monitor está usando el usuario y sus medidas exactas
    int monitor = GetCurrentMonitor();
    int anchoPantalla = GetMonitorWidth(monitor);
    int altoPantalla = GetMonitorHeight(monitor);

    // 4. Redimensionar la ventana a esas medidas y pegarla en la esquina superior izquierda
    SetWindowSize(anchoPantalla, altoPantalla);
    SetWindowPosition(0, 0);

    // --- ACTIVAMOS EL BLOQUEO DE TECLADO ---
    ActivarSeguridadTeclado();

    // 5. Bucle principal
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Llamamos a la función, pero su código detallado vive tranquilamente en menu.c
        if (DibujarPantallaMenu(miIcono) == 1) {
            break; 
        }

        EndDrawing();
    }

    // --- DESACTIVAMOS EL BLOQUEO ---
    DesactivarSeguridadTeclado();

    UnloadTexture(miIcono);
    CloseWindow();
    return 0;
}