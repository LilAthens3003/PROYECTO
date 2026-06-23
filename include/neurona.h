/* ================================================================
 neurona.h - Módulo de Neurona Artificial
 =============================================================
 
 DESCRIPCIÓN:
Este módulo implementa una neurona artificial que puede:
Procesar imágenes representadas como vectores numéricos
Aprender mediante forward y backward propagation
Adaptarse a cualquier tamaño de entrada (dinámico)
Clasificar resultados usando función de activación SIGMOID

 AUTOR: Aarón Guilarte
 VERSIÓN: 1.0
========================================================*/

#ifndef NEURONA_H
#define NEURONA_H

// ================================================================
// 1. BIBLIOTECAS ESTÁNDAR NECESARIAS
// ================================================================
#include <stdio.h>    // Para operaciones de archivo (printf, fopen, etc.)
#include <stdlib.h>   // Para memoria dinámica (malloc, free, etc.)
#include <math.h>     // Para funciones matemáticas (exp, pow, etc.)
#include <string.h>   // Para manejo de cadenas (strtok, strcpy, etc.)
#include <time.h>     // Para generar números aleatorios (srand, rand)

// ================================================================
// 2. CONSTANTES Y CONFIGURACIÓN (MODIFICABLE SEGÚN NECESIDAD)
// ================================================================

// Umbral para clasificación binaria
// - Si salida >= UMBRAL_CLASIFICACION → POSITIVO/APROBADO
// - Si salida < UMBRAL_CLASIFICACION → NEGATIVO/NO APROBADO
// ★ MODIFICAR AQUÍ para cambiar el umbral de decisión ★
#define UMBRAL_CLASIFICACION 0.5f

// Tasa de aprendizaje por defecto
// - Controla qué tan rápido aprende la neurona
// - Valores típicos: 0.01 a 0.1
// ★ MODIFICAR AQUÍ para cambiar la velocidad de aprendizaje ★
#define TASA_APRENDIZAJE_DEFECTO 0.05f

// Número máximo de épocas por defecto
// - Cantidad de iteraciones de entrenamiento
// ★ MODIFICAR AQUÍ para cambiar el número de épocas ★
#define EPOCAS_DEFECTO 1000

// Nombre del archivo de configuración
// ★ MODIFICAR AQUÍ si se cambia el nombre del archivo ★
#define ARCHIVO_CONFIGURACION "config.txt"

// Separador de columnas en archivos CSV
// - Por defecto: punto y coma (;)
// - Puede cambiarse a coma (,) si es necesario
// ★ MODIFICAR AQUÍ si el archivo usa otro separador ★
#define SEPARADOR_CSV ';'

// ================================================================
// 3. ESTRUCTURAS DE DATOS
// ================================================================

/**
 * ESTRUCTURA: Neurona
 * 
 * Representa una neurona artificial individual
 * 
 * CAMPOS:
 *   num_entradas    - Cantidad de valores que recibe (ej: píxeles de imagen)
 *   pesos           - Arreglo dinámico de pesos (uno por cada entrada)
 *   bias            - Sesgo de la neurona (ajuste personal)
 *   salida          - Último valor calculado (resultado de forward propagation)
 *   error           - Error cometido en la última predicción
 */
typedef struct {
    int num_entradas;       // Número de entradas que recibe la neurona
    float *pesos;           // Arreglo dinámico: pesos[i] para entrada[i]
    float bias;             // Sesgo (ajuste personal de la neurona)
    float salida;           // Última salida calculada
    float error;            // Último error calculado (esperado - obtenido)
} Neurona;

/**
 * ESTRUCTURA: Capa (OPCIONAL - Preparada para futuro multicapa)
 * 
 * Agrupa múltiples neuronas en una capa
 * 
 * CAMPOS:
 *   num_neuronas  - Cantidad de neuronas en esta capa
 *   neuronas      - Arreglo dinámico de neuronas
 */
typedef struct {
    int num_neuronas;       // Número de neuronas en la capa
    Neurona *neuronas;      // Arreglo dinámico de neuronas
} Capa;

/**
 * ESTRUCTURA: RedNeuronal (OPCIONAL - Preparada para futuro multicapa)
 * 
 * Representa una red neuronal completa con múltiples capas
 * 
 * CAMPOS:
 *   num_capas         - Cantidad de capas en la red
 *   capas             - Arreglo dinámico de capas
 *   tasa_aprendizaje  - Velocidad de aprendizaje de la red
 */
typedef struct {
    int num_capas;              // Número de capas en la red
    Capa *capas;                // Arreglo dinámico de capas
    float tasa_aprendizaje;     // Tasa de aprendizaje de la red
} RedNeuronal;

/**
 * ESTRUCTURA: Configuracion
 * 
 * Almacena la configuración del sistema desde archivo
 * 
 * CAMPOS:
 *   num_entradas        - Número de entradas de la neurona
 *   tasa_aprendizaje    - Tasa de aprendizaje
 *   epocas              - Número de épocas de entrenamiento
 *   archivo_datos       - Nombre del archivo con los datos
 */
typedef struct {
    int num_entradas;           // Número de entradas de la neurona
    float tasa_aprendizaje;     // Tasa de aprendizaje
    int epocas;                 // Número de épocas de entrenamiento
    char archivo_datos[256];    // Nombre del archivo de datos (CSV)
} Configuracion;

/**
 * ESTRUCTURA: Dataset
 * 
 * Almacena los datos de entrenamiento
 * 
 * CAMPOS:
 *   num_muestras        - Cantidad de ejemplos
 *   num_caracteristicas - Cantidad de valores por ejemplo (entradas)
 *   entradas            - Matriz dinámica: [muestra][característica]
 *   salidas             - Vector dinámico: valor esperado para cada muestra
 */
typedef struct {
    int num_muestras;           // Cantidad de ejemplos en el dataset
    int num_caracteristicas;    // Cantidad de valores por ejemplo
    float **entradas;           // Matriz dinámica de entradas
    float *salidas;             // Vector dinámico de salidas esperadas
} Dataset;

// ================================================================
// 4. FUNCIONES DE CREACIÓN Y DESTRUCCIÓN (MEMORIA DINÁMICA)
// ================================================================

/**
 * crear_neurona - Crea una neurona con memoria dinámica
 * 
 * PARÁMETROS:
 *   num_entradas - Cantidad de entradas que recibirá la neurona
 * 
 * RETORNO:
 *   Puntero a Neurona creada, o NULL si falla la asignación
 * 
 * EJEMPLO:
 *   Neurona* n = crear_neurona(25);  // Para imagen de 5×5 píxeles
 */
Neurona* crear_neurona(int num_entradas);

/**
 * liberar_neurona - Libera la memoria de una neurona
 * 
 * PARÁMETROS:
 *   n - Puntero a neurona a liberar
 * 
 * EJEMPLO:
 *   liberar_neurona(n);
 */
void liberar_neurona(Neurona *n);

/**
 * inicializar_pesos_aleatorios - Asigna valores aleatorios a pesos y bias
 * 
 * PARÁMETROS:
 *   n - Puntero a neurona a inicializar
 * 
 * EJEMPLO:
 *   inicializar_pesos_aleatorios(n);
 */
void inicializar_pesos_aleatorios(Neurona *n);

// ================================================================
// 5. FUNCIONES MATEMÁTICAS Y DE ACTIVACIÓN
// ================================================================

/**
 * sigmoid - Función de activación Sigmoid
 * 
 * Convierte cualquier valor a un número entre 0 y 1
 * Útil para clasificación binaria
 * 
 * PARÁMETROS:
 *   x - Valor a transformar
 * 
 * RETORNO:
 *   Valor entre 0 y 1
 * 
 * FÓRMULA: f(x) = 1 / (1 + e^(-x))
 */
float sigmoid(float x);

/**
 * sigmoid_derivada - Derivada de la función Sigmoid
 * 
 * Necesaria para el cálculo de backpropagation
 * 
 * PARÁMETROS:
 *   x - Valor de entrada (salida de sigmoid)
 * 
 * RETORNO:
 *   Derivada en el punto x
 * 
 * FÓRMULA: f'(x) = f(x) * (1 - f(x))
 */
float sigmoid_derivada(float x);

/**
 * producto_punto - Calcula el producto punto de dos vectores
 * 
 * PARÁMETROS:
 *   a - Primer vector
 *   b - Segundo vector
 *   n - Tamaño de los vectores
 * 
 * RETORNO:
 *   Suma de a[i] * b[i] para i = 0 a n-1
 */
float producto_punto(float *a, float *b, int n);

// ================================================================
// 6. FUNCIONES PRINCIPALES DE LA NEURONA (FORWARD / BACKWARD)
// ================================================================

/**
 * forward_propagation - Calcula la salida de la neurona
 * 
 * PROCESO:
 *   1. Multiplica cada entrada por su peso: entrada[i] * pesos[i]
 *   2. Suma todos los resultados
 *   3. Agrega el bias: suma_total + bias
 *   4. Aplica función de activación (Sigmoid)
 * 
 * PARÁMETROS:
 *   n        - Puntero a la neurona
 *   entradas - Arreglo de valores de entrada
 * 
 * RETORNO:
 *   Salida de la neurona (entre 0 y 1 para Sigmoid)
 * 
 * EJEMPLO:
 *   float entradas[] = {0.2, 0.5, 0.1};
 *   float salida = forward_propagation(n, entradas);
 */
float forward_propagation(Neurona *n, float *entradas);

/**
 * backward_propagation - Ajusta pesos y bias según el error
 * 
 * PROCESO:
 *   1. Calcula el error: esperado - obtenido
 *   2. Calcula el delta: error * derivada_sigmoid(salida)
 *   3. Actualiza cada peso: peso + tasa * delta * entrada
 *   4. Actualiza el bias: bias + tasa * delta
 * 
 * PARÁMETROS:
 *   n        - Puntero a la neurona
 *   entradas - Arreglo de valores de entrada (los mismos que en forward)
 *   esperado - Valor que debería haber producido
 *   tasa     - Tasa de aprendizaje (qué tan rápido ajusta)
 * 
 * EJEMPLO:
 *   backward_propagation(n, entradas, 1.0, 0.05);
 */
void backward_propagation(Neurona *n, float *entradas, float esperado, float tasa);

/**
 * entrenar_neurona - Entrena la neurona con un dataset completo
 * 
 * PROCESO:
 *   1. Para cada época:
 *      a. Para cada muestra del dataset:
 *         - Forward propagation
 *         - Backward propagation
 *         - Acumular error
 *      b. Calcular error promedio de la época
 * 
 * PARÁMETROS:
 *   n        - Puntero a la neurona
 *   datos    - Dataset con ejemplos de entrenamiento
 *   epocas   - Número de épocas a entrenar
 *   tasa     - Tasa de aprendizaje
 * 
 * RETORNO:
 *   Arreglo con el historial de errores (debe ser liberado con free)
 *   O NULL si falla
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Imprime progreso cada 10% ★
 */
float* entrenar_neurona(Neurona *n, Dataset *datos, int epocas, float tasa);

/**
 * predecir - Realiza una predicción con la neurona entrenada
 * 
 * PARÁMETROS:
 *   n        - Puntero a la neurona (ya entrenada)
 *   entradas - Arreglo de valores de entrada
 * 
 * RETORNO:
 *   Salida de la neurona (entre 0 y 1)
 * 
 * EJEMPLO:
 *   float resultado = predecir(n, nuevas_entradas);
 */
float predecir(Neurona *n, float *entradas);

/**
 * clasificar - Clasifica una salida según el umbral
 * 
 * PARÁMETROS:
 *   salida - Valor de salida de la neurona (entre 0 y 1)
 * 
 * RETORNO:
 *   1 si salida >= UMBRAL_CLASIFICACION
 *   0 si salida < UMBRAL_CLASIFICACION
 * 
 * ★ MODIFICAR UMBRAL_CLASIFICACION para cambiar el criterio ★
 */
int clasificar(float salida);

// ================================================================
// 7. FUNCIONES DE CONFIGURACIÓN Y ARCHIVOS
// ================================================================

/**
 * leer_configuracion - Lee la configuración desde un archivo
 * 
 * FORMATO DEL ARCHIVO (config.txt):
 *   # Comentarios comienzan con #
 *   NUM_ENTRADAS=25
 *   TASA_APRENDIZAJE=0.05
 *   EPOCAS=1000
 *   ARCHIVO_DATOS=dataset.csv
 * 
 * PARÁMETROS:
 *   nombre_archivo - Nombre del archivo de configuración
 * 
 * RETORNO:
 *   Puntero a Configuracion, o NULL si falla
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Imprime valores cargados ★
 */
Configuracion* leer_configuracion(const char *nombre_archivo);

/**
 * liberar_configuracion - Libera la memoria de la configuración
 * 
 * PARÁMETROS:
 *   config - Puntero a configuración a liberar
 */
void liberar_configuracion(Configuracion *config);

// ================================================================
// 8. FUNCIONES DE DATASET (CARGA Y MANEJO)
// ================================================================

/**
 * cargar_dataset - Carga un dataset desde archivo CSV
 * 
 * FORMATO DEL CSV:
 *   - Cada fila: valores separados por SEPARADOR_CSV
 *   - Último valor: salida esperada
 *   - Ejemplo: 0.2;0.5;0.1;1
 * 
 * PARÁMETROS:
 *   nombre_archivo - Nombre del archivo CSV
 * 
 * RETORNO:
 *   Puntero a Dataset, o NULL si falla
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Muestra datos cargados ★
 */
Dataset* cargar_dataset(const char *nombre_archivo);

/**
 * liberar_dataset - Libera la memoria de un dataset
 * 
 * PARÁMETROS:
 *   datos - Puntero a dataset a liberar
 */
void liberar_dataset(Dataset *datos);

/**
 * normalizar_dataset - Normaliza los datos al rango [0, 1]
 * 
 * Para imágenes: convierte valores 0-255 a 0-1
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset a normalizar
 *   max_valor - Valor máximo original (ej: 255 para imágenes)
 * 
 * EJEMPLO:
 *   normalizar_dataset(datos, 255.0f);
 */
void normalizar_dataset(Dataset *datos, float max_valor);

// ================================================================
// 9. FUNCIONES DE DEPURACIÓN Y VISUALIZACIÓN
// ================================================================

/**
 * imprimir_estado_neurona - Muestra el estado actual de la neurona
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * Útil para verificar que la neurona está funcionando correctamente
 * 
 * PARÁMETROS:
 *   n - Puntero a la neurona
 */
void imprimir_estado_neurona(Neurona *n);

/**
 * imprimir_pesos - Muestra todos los pesos de la neurona
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * Útil para visualizar qué ha aprendido la neurona
 * 
 * PARÁMETROS:
 *   n - Puntero a la neurona
 */
void imprimir_pesos(Neurona *n);

/**
 * imprimir_error_historial - Muestra el historial de errores
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * Útil para ver cómo evoluciona el aprendizaje
 * 
 * PARÁMETROS:
 *   errores      - Arreglo con el historial de errores
 *   num_errores  - Cantidad de errores en el arreglo
 *   mostrar_todos - 1: muestra todos, 0: muestra resumen
 */
void imprimir_error_historial(float *errores, int num_errores, int mostrar_todos);

/**
 * guardar_pesos_archivo - Guarda los pesos en un archivo de texto
 * 
 * ★ FUNCIÓN PARA REPORTES ★
 * 
 * PARÁMETROS:
 *   n            - Puntero a la neurona
 *   nombre_archivo - Nombre del archivo donde guardar
 * 
 * RETORNO:
 *   1 si se guardó correctamente, 0 si falló
 */
int guardar_pesos_archivo(Neurona *n, const char *nombre_archivo);

// ================================================================
// 10. FUNCIONES DE MANEJO DE ERRORES
// ================================================================

/**
 * mostrar_error - Muestra un mensaje de error en pantalla
 * 
 * ★ FUNCIÓN PARA USUARIO FINAL ★
 * 
 * PARÁMETROS:
 *   mensaje - Texto del error a mostrar
 */
void mostrar_error(const char *mensaje);

// ================================================================
// 11. FUNCIONES DE AYUDA Y UTILERÍA
// ================================================================

/**
 * contar_lineas_archivo - Cuenta líneas en un archivo (ignora comentarios)
 * 
 * PARÁMETROS:
 *   archivo - Puntero al archivo abierto
 * 
 * RETORNO:
 *   Número de líneas no vacías y sin comentarios
 */
int contar_lineas_archivo(FILE *archivo);

/**
 * contar_columnas_csv - Cuenta columnas en una línea de CSV
 * 
 * PARÁMETROS:
 *   linea - Línea de texto del archivo CSV
 * 
 * RETORNO:
 *   Número de columnas (valores separados por SEPARADOR_CSV)
 */
int contar_columnas_csv(const char *linea);

// ================================================================
// FIN DEL ARCHIVO neurona.h
// ================================================================

#endif // NEURONA_H