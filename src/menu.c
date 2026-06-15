// ESTA LÍNEA ES LA MAGIA QUE ENCIENDE RAYGUI
#define RAYGUI_IMPLEMENTATION 
#include "../include/raygui.h"
#include "../include/raylib.h"
 // Importamos Raygui aquí si vamos a usar botones




int DibujarPantallaMenu(Texture2D icono) {
    // 1. Agarramos las medidas actuales de la pantalla como bien dedujiste
    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();
    DrawTexture(icono, 50, -20, WHITE);
    // --- CENTRAR EL TEXTO ---
    const char* titulo = "Proyecto Red Neuronal";
    int tamanoFuente = 40 *2;
    int anchoTexto = MeasureText(titulo, tamanoFuente); // ¿Cuántos píxeles mide esta frase?

    // Centro exacto menos la mitad del texto
    int textoX = (anchoPantalla / 2) - (anchoTexto / 2); 
    int textoY = (altoPantalla / 2) - 150; // Lo ponemos un poco más arriba del centro vertical

    DrawText(titulo, textoX, textoY * 0.50, tamanoFuente, BLACK);
    DrawText("ALUMNOS:", anchoPantalla * 0.80, altoPantalla * 0.80, 20, BLACK);
    DrawText("Santiago Docampo V-31610029", anchoPantalla * 0.80, altoPantalla * 0.85, 20, BLACK);
    DrawText("Aaron Guilarte V-29617884", anchoPantalla * 0.80, altoPantalla * 0.90, 20, BLACK);
    // --- CENTRAR EL BOTÓN ---
    int anchoBoton = 350 ;
    int altoBoton = 50 ;

    // Centro exacto menos la mitad del botón
    int botonX = (anchoPantalla / 2) - (anchoBoton / 2);
    int botonY = (altoPantalla / 2) - (altoBoton / 2); // Centro vertical exacto

    if (GuiButton((Rectangle){ botonX, botonY * 0.60, anchoBoton, altoBoton }, "Iniciar Programa")) {
        // Lógica de lo que pasa al hacer clic
    }
    if (GuiButton((Rectangle){ botonX , botonY * 0.75, anchoBoton, altoBoton }, "Cargar Datos o Imagines")) {
        // Lógica de lo que pasa al hacer clic
    }
     if (GuiButton((Rectangle){ botonX , botonY * 0.90, anchoBoton, altoBoton }, "Visualizar Imagen o Matriz")) {
        // Lógica de lo que pasa al hacer clic
    }
     if (GuiButton((Rectangle){ botonX , botonY * 1.05, anchoBoton, altoBoton }, "Entrenar Neurona")) {
        // Lógica de lo que pasa al hacer clic
    }
     if (GuiButton((Rectangle){ botonX , botonY * 1.20, anchoBoton, altoBoton }, "Ver Grafica de Error")) {
        // Lógica de lo que pasa al hacer clic
    }
     if (GuiButton((Rectangle){ botonX , botonY * 1.35, anchoBoton, altoBoton }, "Realizar Prediciones")) {
        // Lógica de lo que pasa al hacer clic
    }
     if (GuiButton((Rectangle){ botonX , botonY * 1.50, anchoBoton, altoBoton }, "Salir del Programa")) {
        return 1; // Indicar que se quiere salir
    }
    return 0; // Indicar que no se quiere salir
}