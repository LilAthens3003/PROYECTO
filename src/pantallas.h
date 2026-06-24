#ifndef PANTALLAS_H
#define PANTALLAS_H

#include "raylib.h"
#include "../include/dataset.h" // NUEVO ARCHIVO DE AARÓN
#include "../include/neurona.h"

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
PantallaActual DibujarPantallaEntrenamiento(Neurona **neurona, Dataset *datos, int *epocaActual, int totalEpocas, bool *entrenando, float *errorActual, float *historialErrores);
PantallaActual DibujarPantallaGrafico(float *historialErrores, int epocasEntrenadas);
PantallaActual DibujarPantallaPrediccion(Neurona *neurona, Dataset *datos);

#endif