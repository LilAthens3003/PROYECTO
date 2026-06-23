#define RAYGUI_IMPLEMENTATION 
#include "../include/raygui.h"
#include "../include/raylib.h"
#include "pantallas.h" // IMPORTANTE: Incluir nuestro .h modificado

PantallaActual DibujarPantallaMenu(Texture2D icono) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();
    DrawTexture(icono, 50, -20, WHITE);
    
    // --- CENTRAR EL TEXTO ---
    const char* titulo = "Proyecto Red Neuronal";
    int tamanoFuente = 80;
    int anchoTexto = MeasureText(titulo, tamanoFuente);

    int textoX = (anchoPantalla / 2) - (anchoTexto / 2); 
    int textoY = (altoPantalla / 2) - 150; 

    DrawText(titulo, textoX, textoY * 0.50, tamanoFuente, BLACK);
    DrawText("ALUMNOS:", anchoPantalla * 0.80, altoPantalla * 0.80, 20, BLACK);
    DrawText("Santiago Docampo V-31610029", anchoPantalla * 0.80, altoPantalla * 0.85, 20, BLACK);
    DrawText("Aaron Guilarte V-29617884", anchoPantalla * 0.80, altoPantalla * 0.90, 20, BLACK);
    
    // --- CENTRAR EL BOTÓN ---
    int anchoBoton = 350;
    int altoBoton = 50;

    int botonX = (anchoPantalla / 2) - (anchoBoton / 2);
    int botonY = (altoPantalla / 2) - (altoBoton / 2); 

    // Variable para saber a dónde ir. Por defecto, nos quedamos en el menú.
    PantallaActual siguientePantalla = PANTALLA_MENU;

    // --- LÓGICA DE BOTONES ---
    
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


// ====================================================================
// ESQUELETOS DE LAS DEMÁS PANTALLAS (Aquí programaremos en el futuro)
// ====================================================================

PantallaActual DibujarPantallaCargar(Dataset **datos) {
    int anchoPantalla = GetScreenWidth();
    
    // Título de la pantalla
    DrawText("GESTOR DE DATOS", (anchoPantalla / 2) - 250, 100, 50, DARKGRAY);

    // Si el dataset es NULL (está vacío), mostramos el botón para cargarlo
    if (*datos == NULL) {
        DrawText("No hay ningún dataset cargado en memoria.", (anchoPantalla / 2) - 265, 200, 25, RED);
        
        // Botón central para ejecutar la función de Aarón
        if (GuiButton((Rectangle){ (anchoPantalla / 2) - 100, 250, 200, 50 }, "Cargar dataset.csv")) {
            *datos = cargar_dataset("dataset.csv"); 
        }
    } 
    // Si ya tiene datos, mostramos un resumen y damos la opción de borrarlo
    else {
        DrawText("¡Dataset cargado exitosamente!", (anchoPantalla / 2) - 250, 200, 25, DARKGREEN);
        
        // Usamos TextFormat para imprimir variables numéricas en Raylib
        DrawText(TextFormat("- Cantidad de muestras: %d", (*datos)->num_muestras), (anchoPantalla / 2) - 250, 250, 25, BLACK);
        DrawText(TextFormat("- Características por muestra: %d", (*datos)->num_caracteristicas), (anchoPantalla / 2) - 250, 280, 25, BLACK);
        
        // Botón para vaciar la memoria (muy importante en C)
        if (GuiButton((Rectangle){ (anchoPantalla / 2) - 100, 350, 200, 40 }, "Liberar Memoria")) {
            liberar_dataset(*datos);
            *datos = NULL; // Lo volvemos a poner nulo
        }
    }

    // Botón para volver atrás
    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_CARGAR;
}


PantallaActual DibujarPantallaVisualizar(Dataset *datos) {
    int anchoPantalla = GetScreenWidth();
    
    DrawText("VISUALIZADOR DE MATRIZ", (anchoPantalla / 2) - 350, 50, 50, DARKGRAY);

    // 1. Verificamos si el usuario olvidó cargar los datos
    if (datos == NULL) {
        DrawText("Error: No hay datos cargados en memoria.", (anchoPantalla / 2) - 252, 250, 25, RED);
        DrawText("Por favor, ve a ''Cargar Datos'' primero.", (anchoPantalla / 2) - 252, 280, 25, GRAY);
    } 
    // 2. Si hay datos, los imprimimos en formato de tabla/matriz
    else {
        DrawText("Vista previa de los datos cargados:", 50, 120, 30, DARKBLUE);
        
        int startY = 170;
        // Mostramos máximo 5 muestras para que quepan bien en la ventana
        int limite = (datos->num_muestras > 5) ? 5 : datos->num_muestras; 
        
        for (int i = 0; i < limite; i++) {
            // Preparamos un texto gigante para la fila
            char lineaTexto[512] = ""; 
            sprintf(lineaTexto, "Fila %d:  [ ", i + 1);
            
            // Concatenamos todas las entradas de esa fila
            for (int j = 0; j < datos->num_caracteristicas; j++) {
                char valor[20];
                // %.0f imprime sin decimales (0 o 1)
                sprintf(valor, "%.0f ", datos->entradas[i][j]); 
                strcat(lineaTexto, valor);
            }
            
            // Concatenamos la salida esperada al final
            strcat(lineaTexto, "]  --->  Esperado: ");
            char salidaStr[20];
            sprintf(salidaStr, "%.0f", datos->salidas[i]);
            strcat(lineaTexto, salidaStr);
            
            // Dibujamos la fila completa en la pantalla
            DrawText(lineaTexto, 50, startY + (i * 40), 25, BLACK);
        }

        // Un mensaje extra si hay más datos ocultos
        if (datos->num_muestras > 5) {
            DrawText(TextFormat("... y %d filas mas.", datos->num_muestras - 5), 50, startY + (5 * 40), 25 , GRAY);
        }
    }

    // Botón para volver atrás (ahora lo ponemos abajo a la izquierda)
    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_VISUALIZAR;
}

PantallaActual DibujarPantallaEntrenamiento(void) {
    DrawText("PANTALLA ENTRENAMIENTO (En construccion)", 200, 200, 30, DARKGRAY);
    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) return PANTALLA_MENU;
    return PANTALLA_ENTRENAMIENTO;
}

PantallaActual DibujarPantallaGrafico(void) {
    DrawText("PANTALLA GRAFICA DE ERROR (En construccion)", 200, 200, 30, DARKGRAY);
    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) return PANTALLA_MENU;
    return PANTALLA_GRAFICO;
}

PantallaActual DibujarPantallaPrediccion(void) {
    DrawText("PANTALLA DE PREDICCIONES (En construccion)", 200, 200, 30, DARKGRAY);
    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) return PANTALLA_MENU;
    return PANTALLA_PREDICCION;
}