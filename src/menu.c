// ESTA LÍNEA ES LA MAGIA QUE ENCIENDE RAYGUI
#define RAYGUI_IMPLEMENTATION 
#include "../include/raygui.h"
#include "../include/raylib.h"
 // Importamos Raygui aquí si vamos a usar botones




void DibujarPantallaMenu(Texture2D icono) {
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

    DrawText(titulo, textoX, textoY, tamanoFuente, BLACK);
    DrawText("ALUMNOS:", anchoPantalla * 0.80, altoPantalla * 0.80, 20, BLACK);
    DrawText("Santiago Docampo V-31610029", anchoPantalla * 0.80, altoPantalla * 0.85, 20, BLACK);
    DrawText("Aaron Guilarte V-29617884", anchoPantalla * 0.80, altoPantalla * 0.90, 20, BLACK);
    // --- CENTRAR EL BOTÓN ---
    int anchoBoton = 200 ;
    int altoBoton = 50 ;

    // Centro exacto menos la mitad del botón
    int botonX = (anchoPantalla / 2) - (anchoBoton / 2);
    int botonY = (altoPantalla / 2) - (altoBoton / 2); // Centro vertical exacto

    if (GuiButton((Rectangle){ botonX, botonY, anchoBoton, altoBoton }, "Iniciar Programa")) {
        // Lógica de lo que pasa al hacer clic
    }
     if (GuiButton((Rectangle){ botonX , botonY + 80, anchoBoton, altoBoton }, "Salir del Programa")) {
        // Lógica de lo que pasa al hacer clic
    }
}