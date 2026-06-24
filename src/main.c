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

    Dataset *miDataset = NULL; 
    Neurona *miNeurona = NULL;
    
    // 2. VARIABLES DE ANIMACIÓN PARA EL ENTRENAMIENTO
    int epocaActual = 0;
    int totalEpocas = 5000;  // Puedes cambiarlo (5000 épocas es buen promedio)
    bool entrenando = false; // ¿Está en Play o en Pausa?
    float errorActual = 1.0f;

    // --- NUEVO: ARREGLO PARA LA GRÁFICA ---
    // Reservamos memoria para guardar el error de cada época (hasta 5000)
    float *historialErrores = (float*)calloc(totalEpocas, sizeof(float));

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
                // Le agregamos historialErrores al final
                estadoPantalla = DibujarPantallaEntrenamiento(&miNeurona, miDataset, &epocaActual, totalEpocas, &entrenando, &errorActual, historialErrores);
                break;
            case PANTALLA_GRAFICO:
                // Le pasamos el historial y hasta qué época ha entrenado
                estadoPantalla = DibujarPantallaGrafico(historialErrores, epocaActual);
                break;
            case PANTALLA_PREDICCION:
                // Le pasamos la neurona y el dataset (solo para leer)
                estadoPantalla = DibujarPantallaPrediccion(miNeurona, miDataset);
                break;
            case PANTALLA_SALIR:
                salirDelPrograma = true; // Esto romperá el bucle while
                break;
        }

        EndDrawing();
    }

    if (miDataset != NULL) liberar_dataset(miDataset);
    if (miNeurona != NULL) liberar_neurona(miNeurona); // Liberamos la neurona
    free(historialErrores);
    DesactivarSeguridadTeclado();
    UnloadTexture(miIcono);
    CloseWindow();
    return 0;
}