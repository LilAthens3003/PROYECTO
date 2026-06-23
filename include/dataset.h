// ================================================================
// dataset.h - Módulo de Carga y Manejo de Datasets
// ================================================================
// 
// DESCRIPCIÓN:
//   Este módulo proporciona funciones para cargar, validar y
//   manipular datasets desde archivos CSV. Incluye validación
//   completa de datos numéricos y manejo de errores.
//
//   CARACTERÍSTICAS:
//   - Carga de archivos CSV con encabezados y comentarios
//   - Validación de valores numéricos (solo floats válidos)
//   - Normalización de datos al rango [0, 1]
//   - Verificación de consistencia de columnas
//   - Funciones de depuración para visualización
//   - Manejo seguro de memoria dinámica
//
// DEPENDENCIAS:
//   - neurona.h (para usar la estructura Dataset)
//   - Bibliotecas estándar de C
//
// AUTOR: Alumno 1 - Matemática y Núcleo de IA
// FECHA: 2026
// VERSIÓN: 1.0
// ================================================================

#ifndef DATASET_H
#define DATASET_H

#include "neurona.h"  // Para usar la estructura Dataset y Configuracion

// ================================================================
// 1. CONSTANTES Y CONFIGURACIÓN (MODIFICABLE SEGÚN NECESIDAD)
// ================================================================

// Separador de columnas en archivos CSV
// ★ MODIFICAR AQUÍ si se usa otro separador (ej: coma ',') ★
#define SEPARADOR_CSV_DATASET ';'

// Valor máximo por defecto para normalización (imágenes 0-255)
// ★ MODIFICAR AQUÍ según el tipo de datos ★
#define VALOR_MAXIMO_DEFECTO 255.0f

// Número máximo de caracteres por línea en el CSV
// ★ MODIFICAR AQUÍ si se tienen líneas muy largas ★
#define MAX_LINEA_CSV 2048

// ================================================================
// 2. ESTRUCTURAS ADICIONALES
// ================================================================

/**
 * ESTRUCTURA: DatasetInfo
 * 
 * Almacena metadatos e información estadística del dataset
 * Útil para depuración y reportes
 * 
 * CAMPOS:
 *   num_muestras       - Total de ejemplos en el dataset
 *   num_caracteristicas - Total de características por muestra
 *   valor_minimo       - Valor mínimo encontrado en los datos
 *   valor_maximo       - Valor máximo encontrado en los datos
 *   media              - Media de todos los valores
 *   desviacion         - Desviación estándar (aproximada)
 */
typedef struct {
    int num_muestras;
    int num_caracteristicas;
    float valor_minimo;
    float valor_maximo;
    float media;
    float desviacion;
} DatasetInfo;

// ================================================================
// 3. FUNCIONES DE CARGA DE DATASET
// ================================================================

/**
 * cargar_dataset - Carga un dataset desde archivo CSV
 * 
 * FORMATO DEL CSV:
 *   - Comentarios: líneas que empiezan con '#'
 *   - Encabezados: se ignoran automáticamente
 *   - Datos: valores separados por SEPARADOR_CSV_DATASET
 *   - Última columna: salida esperada
 *   - Ejemplo:
 *     # Entradas (X1, X2), Salida Esperada
 *     0;0;0
 *     0;1;1
 * 
 * VALIDACIONES REALIZADAS:
 *   - Archivo existe y se puede abrir
 *   - Todas las filas tienen el mismo número de columnas
 *   - Todos los valores son números válidos (float)
 *   - No hay valores vacíos o nulos
 * 
 * PARÁMETROS:
 *   nombre_archivo - Nombre del archivo CSV a cargar
 * 
 * RETORNO:
 *   Puntero a Dataset, o NULL si falla
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Muestra estadísticas del dataset ★
 */
Dataset* cargar_dataset(const char *nombre_archivo);

/**
 * cargar_dataset_desde_config - Carga dataset usando configuración
 * 
 * PARÁMETROS:
 *   config - Puntero a Configuracion con el nombre del archivo
 * 
 * RETORNO:
 *   Puntero a Dataset, o NULL si falla
 */
Dataset* cargar_dataset_desde_config(Configuracion *config);

// ================================================================
// 4. FUNCIONES DE NORMALIZACIÓN
// ================================================================

/**
 * normalizar_dataset - Normaliza los datos al rango [0, 1]
 * 
 * PROCESO:
 *   Para cada valor: valor_normalizado = valor / max_valor
 *   Si max_valor = 0, no se realiza normalización
 * 
 * PARÁMETROS:
 *   datos     - Puntero al dataset a normalizar
 *   max_valor - Valor máximo original (ej: 255 para imágenes)
 * 
 * EJEMPLO:
 *   normalizar_dataset(datos, 255.0f);  // Para imágenes
 *   normalizar_dataset(datos, 1.0f);    // Si ya están normalizados
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Muestra estadísticas después de normalizar ★
 */
void normalizar_dataset(Dataset *datos, float max_valor);

/**
 * normalizar_dataset_auto - Normaliza detectando el máximo automáticamente
 * 
 * Encuentra el valor máximo en el dataset y normaliza con ese valor
 * Útil cuando no se conoce el rango de los datos
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset a normalizar
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Muestra el máximo detectado ★
 */
void normalizar_dataset_auto(Dataset *datos);

/**
 * desnormalizar_dataset - Restaura los valores originales
 * 
 * PARÁMETROS:
 *   datos     - Puntero al dataset a desnormalizar
 *   max_valor - Valor máximo original (el mismo usado en normalizar)
 */
void desnormalizar_dataset(Dataset *datos, float max_valor);

// ================================================================
// 5. FUNCIONES DE CONSULTA Y MANIPULACIÓN
// ================================================================

/**
 * obtener_num_muestras - Retorna el número de muestras
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset
 * 
 * RETORNO:
 *   Número de muestras, o -1 si el dataset es NULL
 */
int obtener_num_muestras(Dataset *datos);

/**
 * obtener_num_caracteristicas - Retorna el número de características
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset
 * 
 * RETORNO:
 *   Número de características, o -1 si el dataset es NULL
 */
int obtener_num_caracteristicas(Dataset *datos);

/**
 * obtener_muestra - Obtiene una muestra específica del dataset
 * 
 * PARÁMETROS:
 *   datos  - Puntero al dataset
 *   indice - Índice de la muestra (0 = primera)
 * 
 * RETORNO:
 *   Puntero al arreglo de entradas, o NULL si el índice es inválido
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Verifica que el índice sea válido ★
 */
float* obtener_muestra(Dataset *datos, int indice);

/**
 * obtener_salida_esperada - Obtiene la salida esperada de una muestra
 * 
 * PARÁMETROS:
 *   datos  - Puntero al dataset
 *   indice - Índice de la muestra (0 = primera)
 * 
 * RETORNO:
 *   Valor de salida esperada, o -1.0f si el índice es inválido
 */
float obtener_salida_esperada(Dataset *datos, int indice);

/**
 * obtener_estadisticas_dataset - Calcula estadísticas del dataset
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset
 * 
 * RETORNO:
 *   Estructura DatasetInfo con estadísticas calculadas
 * 
 * ★ FUNCIÓN DE DEPURACIÓN: Útil para reportes ★
 */
DatasetInfo obtener_estadisticas_dataset(Dataset *datos);

// ================================================================
// 6. FUNCIONES DE VALIDACIÓN (INTERNAS - EXPUESTAS PARA DEPURACIÓN)
// ================================================================

/**
 * validar_dataset - Verifica la integridad del dataset
 * 
 * VALIDACIONES REALIZADAS:
 *   - Dataset no es NULL
 *   - Todas las filas tienen el mismo número de columnas
 *   - No hay valores NaN o infinitos
 *   - Las salidas están en el rango esperado [0, 1]
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset a validar
 * 
 * RETORNO:
 *   1 si el dataset es válido, 0 si no
 */
int validar_dataset(Dataset *datos);

// ================================================================
// 7. FUNCIONES DE DEPURACIÓN Y VISUALIZACIÓN
// ================================================================

/**
 * imprimir_dataset - Muestra el contenido del dataset
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * Útil para verificar que los datos se cargaron correctamente
 * 
 * PARÁMETROS:
 *   datos        - Puntero al dataset
 *   max_muestras - Número máximo de muestras a mostrar (-1 = todas)
 * 
 * EJEMPLO:
 *   imprimir_dataset(datos, 5);  // Muestra las primeras 5 muestras
 *   imprimir_dataset(datos, -1); // Muestra todas las muestras
 */
void imprimir_dataset(Dataset *datos, int max_muestras);

/**
 * imprimir_estadisticas_dataset - Muestra estadísticas del dataset
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * Muestra: número de muestras, características, rangos, etc.
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset
 */
void imprimir_estadisticas_dataset(Dataset *datos);

/**
 * imprimir_matriz_como_imagen - Muestra una matriz como imagen ASCII
 * 
 * ★ FUNCIÓN DE DEPURACIÓN ★
 * Útil para visualizar imágenes cargadas como dataset
 * 
 * PARÁMETROS:
 *   datos      - Puntero al dataset
 *   indice     - Índice de la muestra a mostrar
 *   filas      - Número de filas de la imagen
 *   columnas   - Número de columnas de la imagen
 * 
 * REQUIERE: filas × columnas = num_caracteristicas
 */
void imprimir_matriz_como_imagen(Dataset *datos, int indice, int filas, int columnas);

// ================================================================
// 8. FUNCIONES DE MEMORIA
// ================================================================

/**
 * liberar_dataset - Libera toda la memoria asociada al dataset
 * 
 * ★ IMPORTANTE: Siempre llamar al finalizar el programa ★
 * 
 * PARÁMETROS:
 *   datos - Puntero al dataset a liberar
 */
void liberar_dataset(Dataset *datos);

/**
 * liberar_dataset_seguro - Libera memoria y pone el puntero a NULL
 * 
 * PARÁMETROS:
 *   datos - Puntero al puntero del dataset (doble puntero)
 * 
 * EJEMPLO:
 *   liberar_dataset_seguro(&datos);  // datos queda en NULL
 */
void liberar_dataset_seguro(Dataset **datos);

// ================================================================
// FIN DEL ARCHIVO dataset.h
// ================================================================

#endif // DATASET_H