// ================================================================
// matematica.h - Módulo de Funciones Matemáticas para IA
// ================================================================
// 
// DESCRIPCIÓN:
//   Este módulo proporciona funciones matemáticas especializadas
//   para el funcionamiento de redes neuronales artificiales.
//   Incluye funciones de activación, operaciones vectoriales,
//   matriciales y estadísticas.
//
//   CARACTERÍSTICAS:
//   - Funciones de activación: Sigmoid, ReLU, Tanh
//   - Derivadas para backpropagation
//   - Operaciones vectoriales y matriciales
//   - Funciones estadísticas para análisis de datos
//   - Utilidades matemáticas generales
//
// DEPENDENCIAS:
//   - math.h (para funciones como exp, sqrt, pow)
//   - stdlib.h (para malloc, free en funciones de matrices)
//
// AUTOR: Aarón Guilarte
// VERSIÓN: 1.0
// ================================================================

#ifndef MATEMATICA_H
#define MATEMATICA_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// ================================================================
// 1. CONSTANTES Y CONFIGURACIÓN
// ================================================================

// ★ MODIFICAR AQUÍ para cambiar la precisión de decimales ★
#define PRECISION_DECIMAL_MAT 100.0f

// Límites para evitar overflow en funciones exponenciales
// ★ MODIFICAR AQUÍ si se necesita mayor o menor rango ★
#define LIMITE_EXP_SUPERIOR 50.0f
#define LIMITE_EXP_INFERIOR -50.0f

// Valor de épsilon para evitar divisiones por cero
#define EPSILON 1e-8f

// ================================================================
// 2. FUNCIONES DE ACTIVACIÓN
// ================================================================

/**
 * sigmoid - Función de activación Sigmoid
 * 
 * Convierte cualquier valor a un número entre 0 y 1
 * Útil para clasificación binaria
 * 
 * FÓRMULA: f(x) = 1 / (1 + e^(-x))
 * 
 * PARÁMETROS:
 *   x - Valor a transformar
 * 
 * RETORNO:
 *   Valor entre 0 y 1
 */
float sigmoid_mat(float x);

/**
 * sigmoid_derivada - Derivada de la función Sigmoid
 * 
 * Necesaria para el cálculo de backpropagation
 * 
 * FÓRMULA: f'(x) = f(x) * (1 - f(x))
 * 
 * PARÁMETROS:
 *   x - Valor de entrada (salida de sigmoid)
 * 
 * RETORNO:
 *   Derivada en el punto x
 */
float sigmoid_derivada_mat(float x);

/**
 * relu - Función de activación ReLU (Rectified Linear Unit)
 * 
 * Deja pasar solo valores positivos
 * Útil para capas ocultas en redes profundas
 * 
 * FÓRMULA: f(x) = max(0, x)
 * 
 * PARÁMETROS:
 *   x - Valor a transformar
 * 
 * RETORNO:
 *   x si x > 0, 0 en caso contrario
 */
float relu_mat(float x);

/**
 * relu_derivada - Derivada de la función ReLU
 * 
 * FÓRMULA: f'(x) = 1 si x > 0, 0 si x < 0
 * 
 * PARÁMETROS:
 *   x - Valor de entrada
 * 
 * RETORNO:
 *   1 si x > 0, 0 en caso contrario
 */
float relu_derivada_mat(float x);

/**
 * tanh_mat - Función de activación Tangente Hiperbólica
 * 
 * Convierte cualquier valor a un número entre -1 y 1
 * Útil para redes que necesitan valores negativos
 * 
 * FÓRMULA: f(x) = (e^x - e^(-x)) / (e^x + e^(-x))
 * 
 * PARÁMETROS:
 *   x - Valor a transformar
 * 
 * RETORNO:
 *   Valor entre -1 y 1
 */
float tanh_mat(float x);

/**
 * tanh_derivada - Derivada de la Tangente Hiperbólica
 * 
 * FÓRMULA: f'(x) = 1 - f(x)²
 * 
 * PARÁMETROS:
 *   x - Valor de entrada
 * 
 * RETORNO:
 *   Derivada en el punto x
 */
float tanh_derivada_mat(float x);

/**
 * softmax - Función de activación Softmax
 * 
 * Convierte un vector en probabilidades que suman 1
 * Útil para clasificación multiclase
 * 
 * PARÁMETROS:
 *   entradas - Arreglo de valores de entrada
 *   n - Tamaño del arreglo
 *   salidas - Arreglo donde guardar los resultados
 * 
 * ★ MODIFICAR AQUÍ para cambiar el comportamiento ★
 */
void softmax_mat(float *entradas, int n, float *salidas);

// ================================================================
// 3. OPERACIONES VECTORIALES
// ================================================================

/**
 * producto_punto_mat - Calcula el producto punto de dos vectores
 * 
 * FÓRMULA: Σ(a[i] * b[i]) para i = 0 a n-1
 * 
 * PARÁMETROS:
 *   a - Primer vector
 *   b - Segundo vector
 *   n - Tamaño de los vectores
 * 
 * RETORNO:
 *   Suma de los productos
 */
float producto_punto_mat(float *a, float *b, int n);

/**
 * sumar_vectores - Suma dos vectores elemento a elemento
 * 
 * PARÁMETROS:
 *   a - Primer vector
 *   b - Segundo vector
 *   n - Tamaño de los vectores
 *   resultado - Arreglo donde guardar la suma
 */
void sumar_vectores_mat(float *a, float *b, int n, float *resultado);

/**
 * restar_vectores - Resta dos vectores elemento a elemento
 * 
 * PARÁMETROS:
 *   a - Primer vector
 *   b - Segundo vector
 *   n - Tamaño de los vectores
 *   resultado - Arreglo donde guardar la resta (a - b)
 */
void restar_vectores_mat(float *a, float *b, int n, float *resultado);

/**
 * multiplicar_escalar_vector - Multiplica un vector por un escalar
 * 
 * PARÁMETROS:
 *   vector - Vector a multiplicar
 *   n - Tamaño del vector
 *   escalar - Valor a multiplicar
 *   resultado - Arreglo donde guardar el resultado
 */
void multiplicar_escalar_vector_mat(float *vector, int n, float escalar, float *resultado);

/**
 * norma_vector - Calcula la norma (magnitud) de un vector
 * 
 * FÓRMULA: ||v|| = sqrt(Σ(v[i]²))
 * 
 * PARÁMETROS:
 *   vector - Vector a evaluar
 *   n - Tamaño del vector
 * 
 * RETORNO:
 *   Norma del vector
 */
float norma_vector_mat(float *vector, int n);

/**
 * normalizar_vector - Normaliza un vector a norma unitaria
 * 
 * PARÁMETROS:
 *   vector - Vector a normalizar
 *   n - Tamaño del vector
 *   resultado - Arreglo donde guardar el vector normalizado
 * 
 * RETORNO:
 *   1 si se normalizó correctamente, 0 si el vector es nulo
 */
int normalizar_vector_mat(float *vector, int n, float *resultado);

// ================================================================
// 4. OPERACIONES MATRICIALES
// ================================================================

/**
 * crear_matriz - Crea una matriz con memoria dinámica
 * 
 * PARÁMETROS:
 *   filas - Número de filas
 *   columnas - Número de columnas
 * 
 * RETORNO:
 *   Puntero a la matriz (float**), o NULL si falla
 * 
 * ★ FUNCIÓN DE MEMORIA: Liberar con liberar_matriz_mat ★
 */
float** crear_matriz_mat(int filas, int columnas);

/**
 * liberar_matriz - Libera la memoria de una matriz
 * 
 * PARÁMETROS:
 *   matriz - Puntero a la matriz a liberar
 *   filas - Número de filas
 */
void liberar_matriz_mat(float **matriz, int filas);

/**
 * multiplicar_matrices - Multiplica dos matrices (A × B)
 * 
 * REQUIERE: columnas_A == filas_B
 * 
 * PARÁMETROS:
 *   A - Primera matriz (filas_A × columnas_A)
 *   filas_A - Filas de A
 *   columnas_A - Columnas de A
 *   B - Segunda matriz (filas_B × columnas_B)
 *   filas_B - Filas de B
 *   columnas_B - Columnas de B
 *   resultado - Matriz donde guardar el producto (filas_A × columnas_B)
 * 
 * RETORNO:
 *   1 si se multiplicó correctamente, 0 si hay error
 */
int multiplicar_matrices_mat(float **A, int filas_A, int columnas_A,
                              float **B, int filas_B, int columnas_B,
                              float **resultado);

/**
 * transponer_matriz - Transpone una matriz
 * 
 * PARÁMETROS:
 *   matriz - Matriz original
 *   filas - Filas de la matriz
 *   columnas - Columnas de la matriz
 *   resultado - Matriz donde guardar la transpuesta (columnas × filas)
 */
void transponer_matriz_mat(float **matriz, int filas, int columnas, float **resultado);

/**
 * imprimir_matriz - Muestra una matriz en consola
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * 
 * PARÁMETROS:
 *   matriz - Matriz a mostrar
 *   filas - Filas de la matriz
 *   columnas - Columnas de la matriz
 *   nombre - Nombre de la matriz (para identificación)
 */
void imprimir_matriz_mat(float **matriz, int filas, int columnas, const char *nombre);

// ================================================================
// 5. FUNCIONES ESTADÍSTICAS
// ================================================================

/**
 * media_vector - Calcula la media de un vector
 * 
 * PARÁMETROS:
 *   vector - Vector a evaluar
 *   n - Tamaño del vector
 * 
 * RETORNO:
 *   Media de los valores
 */
float media_vector_mat(float *vector, int n);

/**
 * varianza_vector - Calcula la varianza de un vector
 * 
 * PARÁMETROS:
 *   vector - Vector a evaluar
 *   n - Tamaño del vector
 *   media - Media del vector (si ya está calculada)
 * 
 * RETORNO:
 *   Varianza de los valores
 */
float varianza_vector_mat(float *vector, int n, float media);

/**
 * desviacion_vector - Calcula la desviación estándar de un vector
 * 
 * PARÁMETROS:
 *   vector - Vector a evaluar
 *   n - Tamaño del vector
 * 
 * RETORNO:
 *   Desviación estándar de los valores
 */
float desviacion_vector_mat(float *vector, int n);

/**
 * maximo_vector - Encuentra el valor máximo de un vector
 * 
 * PARÁMETROS:
 *   vector - Vector a evaluar
 *   n - Tamaño del vector
 * 
 * RETORNO:
 *   Valor máximo
 */
float maximo_vector_mat(float *vector, int n);

/**
 * minimo_vector - Encuentra el valor mínimo de un vector
 * 
 * PARÁMETROS:
 *   vector - Vector a evaluar
 *   n - Tamaño del vector
 * 
 * RETORNO:
 *   Valor mínimo
 */
float minimo_vector_mat(float *vector, int n);

// ================================================================
// 6. FUNCIONES DE UTILERÍA
// ================================================================

/**
 * truncar_decimales - Trunca un número a N decimales
 * 
 * PARÁMETROS:
 *   valor - Número a truncar
 *   decimales - Número de decimales a mantener
 * 
 * RETORNO:
 *   Número truncado
 * 
 * ★ MODIFICAR PRECISION_DECIMAL_MAT para cambiar precisión ★
 */
float truncar_decimales_mat(float valor, int decimales);

/**
 * clamp - Limita un valor entre un mínimo y un máximo
 * 
 * PARÁMETROS:
 *   valor - Valor a limitar
 *   min - Límite inferior
 *   max - Límite superior
 * 
 * RETORNO:
 *   Valor limitado al rango [min, max]
 */
float clamp_mat(float valor, float min, float max);

/**
 * es_numero_valido - Verifica si un número es válido (no NaN, no infinito)
 * 
 * PARÁMETROS:
 *   valor - Valor a verificar
 * 
 * RETORNO:
 *   1 si es válido, 0 si no
 */
int es_numero_valido_mat(float valor);

/**
 * escalar_xor - Función XOR para pruebas
 * 
 * PARÁMETROS:
 *   a - Primer valor (0 o 1)
 *   b - Segundo valor (0 o 1)
 * 
 * RETORNO:
 *   Resultado de XOR (0 o 1)
 */
float escalar_xor_mat(float a, float b);

// ================================================================
// FIN DEL ARCHIVO matematica.h
// ================================================================

#endif // MATEMATICA_H