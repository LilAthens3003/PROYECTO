#include <stdbool.h> // Para usar variables 'bool' true/false
#include "../include/raylib.h"
#include "../include/raygui.h"
#include "pantallas.h"
#include "seguridad.h" 
#include "../include/neurona.h" // Agrega esto arriba para reconocer el Dataset

int main(void) {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED); 

    InitWindow(800, 600, "proyecto");
    Texture2D miIcono = LoadTexture("assets/logo.png");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    int monitor = GetCurrentMonitor();
    int anchoPantalla = GetMonitorWidth(monitor);
    int altoPantalla = GetMonitorHeight(monitor);

    SetWindowSize(anchoPantalla, altoPantalla);
    SetWindowPosition(0, 0);

    ActivarSeguridadTeclado();

    // 1. CREAMOS EL ESPACIO EN MEMORIA PARA LOS DATOS (Inicia vacío)
    Dataset *miDataset = NULL;

    // 1. Iniciamos la máquina de estados en el Menú
    PantallaActual estadoPantalla = PANTALLA_MENU;
    bool salirDelPrograma = false; // Bandera para saber cuándo cerrar

    // 2. Modificamos el bucle para que se detenga si salirDelPrograma es true
    while (!WindowShouldClose() && !salirDelPrograma) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 3. El switch evalúa en qué pantalla estamos y la dibuja
        switch (estadoPantalla) {
            case PANTALLA_MENU:
                estadoPantalla = DibujarPantallaMenu(miIcono);
                break;
            case PANTALLA_CARGAR:
                estadoPantalla = DibujarPantallaCargar(&miDataset); 
                break;
            case PANTALLA_VISUALIZAR:
                estadoPantalla = DibujarPantallaVisualizar(miDataset);
                break;
            case PANTALLA_ENTRENAMIENTO:
                estadoPantalla = DibujarPantallaEntrenamiento();
                break;
            case PANTALLA_GRAFICO:
                estadoPantalla = DibujarPantallaGrafico();
                break;
            case PANTALLA_PREDICCION:
                estadoPantalla = DibujarPantallaPrediccion();
                break;
            case PANTALLA_SALIR:
                salirDelPrograma = true; // Esto romperá el bucle while
                break;
        }

        EndDrawing();
    }

    // 3. LIMPIEZA FINAL: Si el usuario cierra el programa sin borrar los datos, los borramos aquí
    if (miDataset != NULL) {
        liberar_dataset(miDataset);
    }

    DesactivarSeguridadTeclado();
    UnloadTexture(miIcono);
    CloseWindow();
    return 0;
}