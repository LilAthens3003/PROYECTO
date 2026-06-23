#ifndef PANTALLAS_H
#define PANTALLAS_H

#include "raylib.h"
#include "../include/neurona.h" // Importamos la estructura Dataset de Aarón

typedef enum {
    PANTALLA_MENU,
    PANTALLA_CARGAR,
    PANTALLA_VISUALIZAR,
    PANTALLA_ENTRENAMIENTO,
    PANTALLA_GRAFICO,
    PANTALLA_PREDICCION,
    PANTALLA_SALIR
} PantallaActual;

PantallaActual DibujarPantallaMenu(Texture2D icono);

// Fíjate que ahora recibe un "Doble Puntero". Esto es para poder modificar
// la variable original que vivirá en el main.c
PantallaActual DibujarPantallaCargar(Dataset **datos); 


PantallaActual DibujarPantallaVisualizar(Dataset *datos);
PantallaActual DibujarPantallaEntrenamiento(void);
PantallaActual DibujarPantallaGrafico(void);
PantallaActual DibujarPantallaPrediccion(void);

#endif