#define RAYGUI_IMPLEMENTATION 
#include "../include/raygui.h"
#include "../include/raylib.h"
#include "pantallas.h"

// Función mágica para centrar textos sin importar su tamaño
void DrawTextCentradoX(const char* texto, int coordenadaY, int tamanoFuente, Color color) {
    int anchoPantalla = GetScreenWidth();
    int anchoTexto = MeasureText(texto, tamanoFuente);
    int x = (anchoPantalla / 2) - (anchoTexto / 2);
    DrawText(texto, x, coordenadaY, tamanoFuente, color);
}

PantallaActual DibujarPantallaMenu(Texture2D icono) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();
    
    DrawTexture(icono, 50, -20, WHITE);
    
    // --- TÍTULO PRINCIPAL (100) ---
    DrawTextCentradoX("Proyecto Red Neuronal", altoPantalla * 0.15, 100, BLACK);

    // --- PÁRRAFOS (40) Y DETALLES (20) ---
    // Usamos 40 para la etiqueta y 20 para los nombres
    DrawText("ALUMNOS:", anchoPantalla * 0.75, altoPantalla * 0.80, 40, BLACK);
    DrawText("Santiago Docampo V-31610029", anchoPantalla * 0.75, altoPantalla * 0.86, 30, BLACK);
    DrawText("Aaron Guilarte V-29617884", anchoPantalla * 0.75, altoPantalla * 0.89, 30, BLACK);
    
    // --- CENTRAR LOS BOTONES ---
    int anchoBoton = 350;
    int altoBoton = 50;
    int botonX = (anchoPantalla / 2) - (anchoBoton / 2);
    int botonY = (altoPantalla / 2) - (altoBoton / 2); 

    PantallaActual siguientePantalla = PANTALLA_MENU;

    // Los botones se mantienen en tamaño 20 automáticamente por el GuiSetStyle del main.c
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
// PANTALLAS SECUNDARIAS
// ====================================================================

PantallaActual DibujarPantallaCargar(Dataset **datos) {
    int anchoPantalla = GetScreenWidth();
    
    // Título Principal (100)
    DrawTextCentradoX("GESTOR DE DATOS", 60, 80, DARKGRAY);

    if (*datos == NULL) {
        // Párrafo (40)
        DrawTextCentradoX("No hay ningun dataset cargado en memoria.", 220, 40, RED);
        
        if (GuiButton((Rectangle){ (anchoPantalla / 2) - 100, 300, 200, 50 }, "Cargar dataset.csv")) {
            *datos = cargar_dataset("dataset.csv"); 
        }
    } 
    else {
        // Subtítulo (80)
        DrawTextCentradoX("¡Dataset cargado exitosamente!", 200, 40, DARKGREEN);
        
        // Párrafos (40)
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

PantallaActual DibujarPantallaVisualizar(Dataset *datos) {
    

    // Título Principal (100)
    DrawTextCentradoX("VISUALIZADOR DE MATRIZ", 40, 80, DARKGRAY);

    if (datos == NULL) {
        // Párrafos (40)
        DrawTextCentradoX("Error: No hay datos cargados en memoria.", 250, 40, RED);
        DrawTextCentradoX("Por favor, ve a 'Cargar Datos' primero.", 310, 40, GRAY);
    } 
    else {
        // Subtítulo (80)
        DrawText("Vista previa de los datos:", 50, 160, 40, DARKBLUE);
        
        int startY = 260; // Lo bajamos para que no pise al subtítulo
        int limite = (datos->num_muestras > 5) ? 5 : datos->num_muestras; 
        
        for (int i = 0; i < limite; i++) {
            char lineaTexto[512] = ""; 
            sprintf(lineaTexto, "Fila %d:  [ ", i + 1);
            
            for (int j = 0; j < datos->num_caracteristicas; j++) {
                char valor[20];
                sprintf(valor, "%.0f ", datos->entradas[i][j]); 
                strcat(lineaTexto, valor);
            }
            
            strcat(lineaTexto, "]  --->  Esperado: ");
            char salidaStr[20];
            sprintf(salidaStr, "%.0f", datos->salidas[i]);
            strcat(lineaTexto, salidaStr);
            
            // Párrafos (40)
            DrawText(lineaTexto, 50, startY + (i * 50), 30, BLACK);
        }

        if (datos->num_muestras > 5) {
            // Párrafo (40)
            DrawText(TextFormat("... y %d filas mas.", datos->num_muestras - 5), 50, startY + (5 * 50), 30, GRAY);
        }
    }

    if (GuiButton((Rectangle){ 50, 50, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_VISUALIZAR;
}

PantallaActual DibujarPantallaEntrenamiento(Neurona **neurona, Dataset *datos, int *epocaActual, int totalEpocas, bool *entrenando, float *errorActual, float *historialErrores) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();
    
    // Título Principal (100) - NOTA: Si ves que no cabe en la pantalla, puedes bajarlo a 80.
    DrawTextCentradoX("ENTRENAMIENTO DE LA RED", 40, 80, DARKGRAY);

    if (datos == NULL) {
        // Párrafo (40)
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
        int epocasPorFrame = 10; 
        for(int paso = 0; paso < epocasPorFrame && *epocaActual < totalEpocas; paso++) {
            float error_total = 0.0f;
            for (int i = 0; i < datos->num_muestras; i++) {
                forward_propagation(*neurona, datos->entradas[i]);
                backward_propagation(*neurona, datos->entradas[i], datos->salidas[i], 0.1f); 
                error_total += (*neurona)->error * (*neurona)->error;
            }
            *errorActual = error_total / datos->num_muestras;
            historialErrores[*epocaActual] = *errorActual; // <--- AGREGA ESTO AQUÍ
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

    // Párrafo (40)
    DrawTextCentradoX("Progreso de Epocas:", 320, 40, BLACK);
    GuiProgressBar((Rectangle){ centroX, 380, 400, 30 }, "0", TextFormat("%d", totalEpocas), (float*)epocaActual, 0, (float)totalEpocas);
    
    // Párrafos (40)
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

PantallaActual DibujarPantallaGrafico(float *historialErrores, int epocasEntrenadas) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();

    DrawTextCentradoX("CURVA DE APRENDIZAJE (ERROR)", 40, 80, DARKGRAY);

    if (epocasEntrenadas <= 1) {
        DrawTextCentradoX("Aun no hay datos suficientes para graficar.", 300, 40, RED);
        DrawTextCentradoX("Ve a 'Entrenar Neurona' primero.", 360, 30, GRAY);
    } else {
        // --- MEDIDAS DEL PLANO CARTESIANO ---
        int margenIzq = 150;
        int margenAbajo = 150;
        int anchoGrafica = anchoPantalla - margenIzq - 100; // Espacio horizontal
        int altoGrafica = altoPantalla - 250;               // Espacio vertical
        
        int origenX = margenIzq;
        int origenY = altoPantalla - margenAbajo;

        // --- DIBUJAR LOS EJES (Líneas Negras) ---
        // Eje Y (Vertical)
        DrawLineEx((Vector2){origenX, origenY}, (Vector2){origenX, origenY - altoGrafica}, 3, BLACK);
        // Eje X (Horizontal)
        DrawLineEx((Vector2){origenX, origenY}, (Vector2){origenX + anchoGrafica, origenY}, 3, BLACK);

        // --- ETIQUETAS DE LOS EJES ---
        DrawText("Error (MSE)", origenX - 120, origenY - 20 - altoGrafica, 20, BLACK);
        DrawText("Epocas", origenX + anchoGrafica + 10, origenY - 10, 20, BLACK);
        
        // Marcadores de Error (Eje Y)
        DrawText("1.0 -", origenX - 45, origenY - altoGrafica, 20, GRAY);
        DrawText("0.5 -", origenX - 45, origenY - (altoGrafica / 2), 20, GRAY);
        DrawText("0.0 -", origenX - 45, origenY - 10, 20, GRAY);

        // --- DIBUJAR LA LÍNEA DEL HISTORIAL DE ERRORES ---
        // Buscamos el mayor error para escalar la gráfica (suele ser el inicial)
        float maxError = 1.0f; // Escala máxima

        for (int i = 0; i < epocasEntrenadas - 1; i++) {
            // Calcular coordenadas X (separación proporcional a la cantidad de épocas)
            float x1 = origenX + ((float)i / epocasEntrenadas) * anchoGrafica;
            float x2 = origenX + ((float)(i + 1) / epocasEntrenadas) * anchoGrafica;

            // Calcular coordenadas Y (invertidas porque Y crece hacia abajo en la pantalla)
            float y1 = origenY - ((historialErrores[i] / maxError) * altoGrafica);
            float y2 = origenY - ((historialErrores[i+1] / maxError) * altoGrafica);

            // Dibujar el segmento de línea entre la época actual y la siguiente
            DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 2, BLUE);
        }
    }

    if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) {
        return PANTALLA_MENU;
    }
    
    return PANTALLA_GRAFICO;
}

PantallaActual DibujarPantallaPrediccion(Neurona *neurona, Dataset *datos) {
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();

    // Título Principal (100) - Lo bajamos a 80 si es muy largo
    DrawTextCentradoX("MOTOR DE PREDICCIONES", 40, 80, DARKGRAY);

    // 1. SEGURIDAD: Verificar que la IA esté lista
    if (neurona == NULL || datos == NULL) {
        DrawTextCentradoX("Error: La neurona no ha sido entrenada aun.", 250, 40, RED);
        DrawTextCentradoX("Ve a 'Entrenar Neurona' primero.", 310, 30, GRAY);
        
        if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) return PANTALLA_MENU;
        return PANTALLA_PREDICCION;
    }

    // 2. VARIABLES DE INTERFAZ (Se guardan en memoria gracias a 'static')
    // Preparamos hasta 10 cajas de texto por si el dataset tiene muchas entradas
    static char textosEntrada[10][20] = { "0", "0", "0", "0", "0", "0", "0", "0", "0", "0" };
    static int cajaActiva = -1; // Para saber en qué caja estamos escribiendo
    static float resultadoPrediccion = -1.0f; // -1 significa que no se ha calculado nada

    DrawTextCentradoX("Ingresa los valores de entrada para que la IA adivine:", 160, 30, DARKBLUE);

    // 3. DIBUJAR CAJAS DE TEXTO DINÁMICAS (Según la cantidad de características)
    int startY = 220;
    int centroX = (anchoPantalla / 2) - 150;

    for (int i = 0; i < datos->num_caracteristicas; i++) {
        DrawText(TextFormat("Entrada (X%d):", i + 1), centroX - 250, startY + (i * 60) + 10, 30, BLACK);
        
        // Dibujamos la caja de texto. Si el usuario le da clic, se activa la edición.
        if (GuiTextBox((Rectangle){ centroX, startY + (i * 60), 300, 40 }, textosEntrada[i], 20, cajaActiva == i)) {
            // Cambiar el estado: si estaba activa la desactiva, si no, la activa
            cajaActiva = (cajaActiva == i) ? -1 : i; 
        }
    }

    // 4. EL BOTÓN MÁGICO QUE HACE PENSAR A LA IA
    int botonY = startY + (datos->num_caracteristicas * 60) + 40;
    if (GuiButton((Rectangle){ centroX, botonY, 300, 50 }, "PREDECIR RESULTADO")) {
        // A) Convertir los textos que escribió el usuario a números (floats)
        float entradas_float[10];
        for (int i = 0; i < datos->num_caracteristicas; i++) {
            entradas_float[i] = (float)atof(textosEntrada[i]); 
        }

        // B) Ejecutar la matemática de Aarón (Solo hacia adelante, sin entrenar)
        resultadoPrediccion = forward_propagation(neurona, entradas_float);
        
        // Quitar la selección de cualquier caja de texto
        cajaActiva = -1; 
    }

    // 5. MOSTRAR EL RESULTADO
    if (resultadoPrediccion >= 0.0f) {
        int resultadoY = botonY + 80;
        DrawTextCentradoX("=====================================", resultadoY, 30, GRAY);
        
        // Imprimir el valor crudo de la neurona
        DrawTextCentradoX(TextFormat("Salida real de la neurona: %.6f", resultadoPrediccion), resultadoY + 40, 30, DARKGRAY);
        
        // Clasificación amigable (Si es mayor a 0.5 asumimos que es 1, sino 0)
        int clasificacion = (resultadoPrediccion >= 0.5f) ? 1 : 0;
        Color colorClasif = (clasificacion == 1) ? DARKGREEN : BLUE;
        
        DrawTextCentradoX(TextFormat("LA IA PREDICE QUE EL RESULTADO ES: %d", clasificacion), resultadoY + 90, 40, colorClasif);
        DrawTextCentradoX("=====================================", resultadoY + 140, 30, GRAY);
    }

    // --- BOTÓN DE VOLVER ---
    if (GuiButton((Rectangle){ 50, altoPantalla - 100, 100, 40 }, "<- Volver")) {
        resultadoPrediccion = -1.0f; // Reiniciamos el resultado al salir
        cajaActiva = -1;
        return PANTALLA_MENU;
    }

    return PANTALLA_PREDICCION;
}