#define RAYGUI_IMPLEMENTATION 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/raygui.h"
#include "../include/raylib.h"
#include "pantallas.h"
#include "seguridad.h"
#include "../include/neurona.h"
#include "../include/dataset.h"

/* Realiza el cálculo del offset horizontal necesario para alinear cadenas de texto
 * respecto al centro absoluto de las coordenadas de la ventana activa. */
void DrawTextCentradoX(const char* texto, int coordenadaY, int tamanoFuente, Color color) {
    int anchoPantalla = GetScreenWidth();
    int anchoTexto = MeasureText(texto, tamanoFuente);
    int x = (anchoPantalla / 2) - (anchoTexto / 2);
    DrawText(texto, x, coordenadaY, tamanoFuente, color);
}

/* Renderiza el panel principal de navegación y evalúa la interacción del usuario
 * para retornar el siguiente estado correspondiente del flujo de la aplicación. */
PantallaActual DibujarPantallaMenu(Texture2D icono) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();
    
    DrawTexture(icono, 50, -20, WHITE);
    
    DrawTextCentradoX("Proyecto Red Neuronal", altoPantalla * 0.15, 100, BLACK);

    DrawText("ALUMNOS:", anchoPantalla * 0.75, altoPantalla * 0.80, 40, BLACK);
    DrawText("Santiago Docampo V-31610029", anchoPantalla * 0.75, altoPantalla * 0.86, 30, BLACK);
    DrawText("Aaron Guilarte V-29617884", anchoPantalla * 0.75, altoPantalla * 0.89, 30, BLACK);
    
    int anchoBoton = 350;
    int altoBoton = 50;
    int botonX = (anchoPantalla / 2) - (anchoBoton / 2);
    int botonY = (altoPantalla / 2) - (altoBoton / 2); 

    PantallaActual siguientePantalla = PANTALLA_MENU;

    if (GuiButton((Rectangle){ botonX , botonY * 0.75, anchoBoton, altoBoton }, "Cargar Datos o Imagenes")) {
        siguientePantalla = PANTALLA_CARGAR;
    }
    if (GuiButton((Rectangle){ botonX , botonY * 0.90, anchoBoton, altoBoton }, "Visualizar Imagen o Matriz")) {
        siguientePantalla = PANTALLA_VISUALIZAR;
    }
    if (GuiButton((Rectangle){ botonX , botonY * 1.05, anchoBoton, altoBoton }, "Entrenar Neurona")) {
        siguientePantalla = PANTALLA_ENTRENAMIENTO;
    }
    if (GuiButton((Rectangle){ botonX , botonY * 1.20, anchoBoton, altoBoton }, "Ver Grafica de Error")) {
        siguientePantalla = PANTALLA_GRAFICO;
    }
    if (GuiButton((Rectangle){ botonX , botonY * 1.35, anchoBoton, altoBoton }, "Realizar Predicciones")) {
        siguientePantalla = PANTALLA_PREDICCION;
    }
    if (GuiButton((Rectangle){ botonX , botonY * 1.50, anchoBoton, altoBoton }, "Salir del Programa")) {
        siguientePantalla = PANTALLA_SALIR;
    }
    
    return siguientePantalla; 
}

/* Gestiona la entrada de datos crudos desde archivos externos, procesando la carga
 * y permitiendo la liberación manual de la estructura de memoria si es requerido. */
PantallaActual DibujarPantallaCargar(Dataset **datos) {
    int anchoPantalla = GetScreenWidth();
    
    DrawTextCentradoX("GESTOR DE DATOS", 60, 80, DARKGRAY);

    if (*datos == NULL) {
        DrawTextCentradoX("No hay ningun dataset cargado en memoria.", 220, 40, RED);
        
        if (GuiButton((Rectangle){ (anchoPantalla / 2) - 100, 300, 200, 50 }, "Cargar dataset.csv")) {
            *datos = cargar_dataset("dataset.csv"); 
        }
    } 
    else {
        DrawTextCentradoX("¡Dataset cargado exitosamente!", 200, 40, DARKGREEN);
        DrawTextCentradoX(TextFormat("- Cantidad de muestras: %d", (*datos)->num_muestras), 320, 30, BLACK);
        DrawTextCentradoX(TextFormat("- Caracteristicas por muestra: %d", (*datos)->num_caracteristicas), 380, 30, BLACK);
        
        if (GuiButton((Rectangle){ (anchoPantalla / 2) - 100, 460, 200, 40 }, "Liberar Memoria")) {
            liberar_dataset(*datos);
            *datos = NULL; 
        }
    }

    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_CARGAR;
}

/* Transforma los valores flotantes del conjunto de datos en una representación visual
 * en formato matriz para auditar la integridad de los datos cargados. */
PantallaActual DibujarPantallaVisualizar(Dataset *datos) {
    DrawTextCentradoX("VISUALIZADOR DE MATRIZ", 40, 80, DARKGRAY);

    if (datos == NULL) {
        DrawTextCentradoX("Error: No hay datos cargados en memoria.", 250, 40, RED);
        DrawTextCentradoX("Por favor, ve a 'Cargar Datos' primero.", 310, 40, GRAY);
    } 
    else {
        DrawText("Vista previa de los datos:", 50, 160, 40, DARKBLUE);
        
        int startY = 260; 
        int limite = (datos->num_muestras > 15) ? 15 : datos->num_muestras; 
        
        for (int i = 0; i < limite; i++) {
            char lineaTexto[512] = ""; 
            sprintf(lineaTexto, "Fila %d:  [ ", i + 1);
            
            for (int j = 0; j < datos->num_caracteristicas; j++) {
                char valor[20];
                sprintf(valor, "%.2f ", datos->entradas[i][j]); 
                strcat(lineaTexto, valor);
            }
            
            strcat(lineaTexto, "]  --->  Esperado: ");
            char salidaStr[20];
            sprintf(salidaStr, "%.0f", datos->salidas[i]);
            strcat(lineaTexto, salidaStr);
            
            DrawText(lineaTexto, 50, startY + (i * 50), 30, BLACK);
        }

        if (datos->num_muestras > 15) {
            DrawText(TextFormat("... y %d filas mas.", datos->num_muestras - 15), 50, startY + (15 * 50), 30, GRAY);
        }
    }

    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_VISUALIZAR;
}

/* Controla el flujo iterativo del algoritmo de backpropagation, repartiendo la carga de
 * procesamiento en fotogramas secuenciales para evitar el bloqueo del hilo principal. */
PantallaActual DibujarPantallaEntrenamiento(Neurona **neurona, Dataset *datos, int *epocaActual, int totalEpocas, bool *entrenando, float *errorActual, float *historialErrores) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();
    
    DrawTextCentradoX("ENTRENAMIENTO DE LA RED", 40, 80, DARKGRAY);

    if (datos == NULL) {
        DrawTextCentradoX("Error: Debes cargar un Dataset primero.", 300, 40, RED);
        if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) return PANTALLA_MENU;
        return PANTALLA_ENTRENAMIENTO;
    }

    if (*neurona == NULL) {
        *neurona = crear_neurona(datos->num_caracteristicas);
        inicializar_pesos_aleatorios(*neurona);
        *epocaActual = 0;
    }

    if (*entrenando && *epocaActual < totalEpocas) {
        int epocasPorFrame = 1; 
        for(int paso = 0; paso < epocasPorFrame && *epocaActual < totalEpocas; paso++) {
            float error_total = 0.0f;
            for (int i = 0; i < datos->num_muestras; i++) {
                forward_propagation(*neurona, datos->entradas[i]);
                backward_propagation(*neurona, datos->entradas[i], datos->salidas[i], 0.1f); 
                error_total += (*neurona)->error * (*neurona)->error;
            }
            *errorActual = error_total / datos->num_muestras;
            historialErrores[*epocaActual] = *errorActual; 
            (*epocaActual)++; 
        }
        if (*epocaActual >= totalEpocas) *entrenando = false; 
    }

    int centroX = (anchoPantalla / 2) - 200; 
    
    const char *textoBoton = (*entrenando) ? "PAUSAR ENTRENAMIENTO" : "INICIAR ENTRENAMIENTO";
    if (*epocaActual >= totalEpocas) textoBoton = "ENTRENAMIENTO FINALIZADO";
    
    if (GuiButton((Rectangle){ centroX, 180, 400, 50 }, textoBoton)) {
        if (*epocaActual < totalEpocas) *entrenando = !(*entrenando);
    }

    if (GuiButton((Rectangle){ centroX, 250, 400, 50 }, "Reiniciar Pesos (Empezar de cero)")) {
        *entrenando = false;
        *epocaActual = 0;
        *errorActual = 1.0f;
        inicializar_pesos_aleatorios(*neurona);
    }

    DrawTextCentradoX("Progreso de Epocas:", 320, 40, BLACK);
    
    float progreso = (float)(*epocaActual);
    if (progreso < 0.0f) progreso = 0.0f;
    if (progreso > (float)totalEpocas) progreso = (float)totalEpocas;
    
    GuiProgressBar((Rectangle){ centroX, 380, 400, 30 }, "0", TextFormat("%d", totalEpocas), &progreso, 0, (float)totalEpocas);
    
    DrawTextCentradoX(TextFormat("Epoca: %d / %d", *epocaActual, totalEpocas), 440, 30, DARKBLUE);
    
    Color colorError = (*errorActual < 0.05f) ? DARKGREEN : RED;
    if (*epocaActual == 0) colorError = GRAY; 
    
    DrawTextCentradoX(TextFormat("Margen de Error Actual: %.6f", *errorActual), 500, 30, colorError);

    if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) {
        *entrenando = false; 
        return PANTALLA_MENU;
    }
    
    return PANTALLA_ENTRENAMIENTO;
}

/* Dibuja un plano cartesiano escalando dinámicamente los ejes según el vector histórico
 * para proyectar el comportamiento de convergencia o divergencia del error cuadrático. */
PantallaActual DibujarPantallaGrafico(float *historialErrores, int epocasEntrenadas) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();

    DrawTextCentradoX("CURVA DE APRENDIZAJE (ERROR)", 40, 80, DARKGRAY);

    if (epocasEntrenadas <= 1) {
        DrawTextCentradoX("Aun no hay datos suficientes para graficar.", 300, 40, RED);
        DrawTextCentradoX("Ve a 'Entrenar Neurona' primero.", 360, 30, GRAY);
    } else {
        int margenIzq = 150;
        int margenAbajo = 150;
        int anchoGrafica = anchoPantalla - margenIzq - 100; 
        int altoGrafica = altoPantalla - 250;               
        
        int origenX = margenIzq;
        int origenY = altoPantalla - margenAbajo;

        DrawLineEx((Vector2){origenX, origenY}, (Vector2){origenX, origenY - altoGrafica}, 3, BLACK);
        DrawLineEx((Vector2){origenX, origenY}, (Vector2){origenX + anchoGrafica, origenY}, 3, BLACK);

        DrawText("Error (MSE)", origenX - 120, (origenY - 20) - altoGrafica, 20, BLACK);
        DrawText("Epocas", origenX + anchoGrafica + 10, origenY - 10, 20, BLACK);
        
        DrawText("1.0 -", origenX - 45, origenY - altoGrafica, 20, GRAY);
        DrawText("0.5 -", origenX - 45, origenY - (altoGrafica / 2), 20, GRAY);
        DrawText("0.0 -", origenX - 45, origenY - 10, 20, GRAY);

        float maxError = 1.0f; 

        for (int i = 0; i < epocasEntrenadas - 1; i++) {
            float x1 = origenX + ((float)i / epocasEntrenadas) * anchoGrafica;
            float x2 = origenX + ((float)(i + 1) / epocasEntrenadas) * anchoGrafica;

            float y1 = origenY - ((historialErrores[i] / maxError) * altoGrafica);
            float y2 = origenY - ((historialErrores[i+1] / maxError) * altoGrafica);

            DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 2, BLUE);
        }
    }

    if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_GRAFICO;
}

/* Captura variables de entrada independientes mediante elementos TextBox de GUI
 * y solicita al modelo entrenado la ejecución de un paso de inferencia estadística. */
PantallaActual DibujarPantallaPrediccion(Neurona *neurona, Dataset *datos) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();

    DrawTextCentradoX("MOTOR DE PREDICCIONES", 40, 80, DARKGRAY);

    if (neurona == NULL || datos == NULL) {
        DrawTextCentradoX("Error: La neurona no ha sido entrenada aun.", 250, 40, RED);
        DrawTextCentradoX("Ve a 'Entrenar Neurona' primero.", 310, 30, GRAY);
        
        if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) return PANTALLA_MENU;
        return PANTALLA_PREDICCION;
    }

    static char textosEntrada[10][20] = { "0", "0", "0", "0", "0", "0", "0", "0", "0", "0" };
    static int cajaActiva = -1; 
    static float resultadoPrediccion = -1.0f; 

    DrawTextCentradoX("Ingresa los valores de entrada para que la neurona adivine:", 160, 30, DARKBLUE);

    int startY = 220;
    int centroX = (anchoPantalla / 2) - 150;

    for (int i = 0; i < datos->num_caracteristicas && i < 10; i++) {
        DrawText(TextFormat("Entrada (X%d):", i + 1), centroX - 180, startY + (i * 60) + 10, 30, BLACK);
        
        if (GuiTextBox((Rectangle){ centroX + 50, startY + (i * 60), 300, 40 }, textosEntrada[i], 20, cajaActiva == i)) {
            cajaActiva = (cajaActiva == i) ? -1 : i; 
        }
    }

    int botonY = startY + (datos->num_caracteristicas * 60) + 40;
    if (GuiButton((Rectangle){ centroX, botonY, 300, 50 }, "PREDECIR RESULTADO")) {
        float entradas_float[10];
        for (int i = 0; i < datos->num_caracteristicas && i < 10; i++) {
            entradas_float[i] = (float)atof(textosEntrada[i]); 
        }

        resultadoPrediccion = forward_propagation(neurona, entradas_float);
        cajaActiva = -1; 
    }

    if (resultadoPrediccion >= 0.0f) {
        int resultadoY = botonY + 80;
        DrawTextCentradoX("=====================================", resultadoY, 30, GRAY);
        
        DrawTextCentradoX(TextFormat("Salida real de la neurona: %.6f", resultadoPrediccion), resultadoY + 40, 30, DARKGRAY);
        
        int clasificacion = (resultadoPrediccion >= 0.5f) ? 1 : 0;
        Color colorClasif = (clasificacion == 1) ? DARKGREEN : BLUE;
        
        DrawTextCentradoX(TextFormat("LA NEURONA PREDICE QUE EL RESULTADO ES: %d", clasificacion), resultadoY + 90, 40, colorClasif);
        DrawTextCentradoX("=====================================", resultadoY + 140, 30, GRAY);
    }

    if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) {
        resultadoPrediccion = -1.0f; 
        cajaActiva = -1;
        return PANTALLA_MENU;
    }

    return PANTALLA_PREDICCION;
}