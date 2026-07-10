#include <stdbool.h> 
#include <stdlib.h>
#include "../include/raylib.h"
#include "../include/raygui.h"
#include "pantallas.h"
#include "seguridad.h" 
#include "../include/neurona.h" 
#include "../include/dataset.h"

int main(void) {
    // Inicialización del contexto gráfico y configuración de la resolución nativa de la pantalla.
    SetConfigFlags(FLAG_WINDOW_UNDECORATED); 
    InitWindow(800, 600, "proyecto");
    Texture2D miIcono = LoadTexture("assets/logo.png");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    int monitor = GetCurrentMonitor();
    int anchoPantalla = GetMonitorWidth(monitor);
    int altoPantalla = GetMonitorHeight(monitor);

    SetWindowSize(anchoPantalla, altoPantalla);
    SetWindowPosition(0, 0);

    //ActivarSeguridadTeclado();

    // Declaración de punteros a estructuras principales para la gestión del modelo en memoria.
    Dataset *miDataset = NULL; 
    Neurona *miNeurona = NULL;
    
    // Parámetros de control para el ciclo de aprendizaje de la red neuronal artificial.
    int epocaActual = 0;
    int totalEpocas = 5000; 
    bool entrenando = false; 
    float errorActual = 1.0f;

    // Asignación de memoria para almacenar la métrica de error histórico a graficar.
    float *historialErrores = (float*)calloc(totalEpocas, sizeof(float));

    // Configuración de la máquina de estados que administra la navegación de la interfaz.
    PantallaActual estadoPantalla = PANTALLA_MENU;
    bool salirDelPrograma = false;

    // Bucle principal de ejecución y renderizado manejado a nivel de fotogramas.
    while (!WindowShouldClose() && !salirDelPrograma) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

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
                estadoPantalla = DibujarPantallaEntrenamiento(&miNeurona, miDataset, &epocaActual, totalEpocas, &entrenando, &errorActual, historialErrores);
                break;
            case PANTALLA_GRAFICO:
                estadoPantalla = DibujarPantallaGrafico(historialErrores, epocaActual);
                break;
            case PANTALLA_PREDICCION:
                estadoPantalla = DibujarPantallaPrediccion(miNeurona, miDataset);
                break;
            case PANTALLA_SALIR:
                salirDelPrograma = true; 
                break;
        }

        EndDrawing();
    }

    // Liberación de recursos y memoria alojada dinámicamente para prevenir perdida de memoria.
    if (miDataset != NULL) liberar_dataset(miDataset);
    if (miNeurona != NULL) liberar_neurona(miNeurona); 
    free(historialErrores);
    
    DesactivarSeguridadTeclado();
    UnloadTexture(miIcono);
    CloseWindow();
    
    return 0;
}