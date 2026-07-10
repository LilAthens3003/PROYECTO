/* Directivas de preprocesador que implementan el patrón Include Guard.
 * Previenen la redefinición de estructuras en memoria durante la fase de enlazado. */
#ifndef PANTALLAS_H
#define PANTALLAS_H

/* Dependencias necesarias para el renderizado gráfico (Raylib) y las estructuras de neurona.
 * Sirve como puente de acoplamiento entre la capa visual y la lógica de negocio. */
#include "raylib.h"
#include "../include/dataset.h" 
#include "../include/neurona.h"

/* Enumeración tipada para gestionar la Máquina de Estados Finitos (FSM) de la ventana.
 * Restringe el flujo de navegación exclusivamente a estos identificadores seguros. */
typedef enum {
    PANTALLA_MENU,
    PANTALLA_CARGAR,
    PANTALLA_VISUALIZAR,
    PANTALLA_ENTRENAMIENTO,
    PANTALLA_GRAFICO,
    PANTALLA_PREDICCION,
    PANTALLA_SALIR
} PantallaActual;

/* Renderiza el panel de inicio. Recibe la textura precargada en VRAM por valor. */
PantallaActual DibujarPantallaMenu(Texture2D icono);

/* Emplea un doble puntero para mutar la dirección de memoria original del dataset 
 * instanciado en el main, permitiendo cargar o liberar datos dinámicamente. */
PantallaActual DibujarPantallaCargar(Dataset **datos); 

/* Proyecta el estado actual del dataset en memoria bajo un formato tabular estático. */
PantallaActual DibujarPantallaVisualizar(Dataset *datos);

/* Orquesta el ciclo de aprendizaje iterativo fragmentando el cálculo por fotogramas.
 * Utiliza punteros mutables para actualizar el progreso y la métrica de convergencia global. */
PantallaActual DibujarPantallaEntrenamiento(Neurona **neurona, Dataset *datos, int *epocaActual, int totalEpocas, bool *entrenando, float *errorActual, float *historialErrores);

/* Renderiza un plano cartesiano para la representación gráfica del Error Cuadrático Medio (MSE). */
PantallaActual DibujarPantallaGrafico(float *historialErrores, int epocasEntrenadas);

/* Módulo de inferencia: evalúa un vector numérico nuevo y ejecuta el forward_propagation. */
PantallaActual DibujarPantallaPrediccion(Neurona *neurona, Dataset *datos);

#endif