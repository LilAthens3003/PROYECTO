// ================================================================
// matematica.c - Implementación del Módulo de Matemáticas
// ================================================================
// 
// DESCRIPCIÓN:
//   Este archivo contiene la implementación completa de todas las
//   funciones declaradas en matematica.h. Incluye funciones de
//   activación, operaciones vectoriales, matriciales y estadísticas.
//
// AUTOR: Aarón Guilarte
// VERSIÓN: 1.0
// ================================================================

#include "../include/matematica.h"
#include <string.h>

// ================================================================
// 1. FUNCIONES DE ACTIVACIÓN
// ================================================================

float sigmoid_mat(float x) {
    // Clamping para evitar overflow en exp()
    if (x > LIMITE_EXP_SUPERIOR) return 1.0f;
    if (x < LIMITE_EXP_INFERIOR) return 0.0f;
    return 1.0f / (1.0f + expf(-x));
}

float sigmoid_derivada_mat(float x) {
    float s = sigmoid_mat(x);
    return s * (1.0f - s);
}

float relu_mat(float x) {
    return (x > 0.0f) ? x : 0.0f;
}

float relu_derivada_mat(float x) {
    return (x > 0.0f) ? 1.0f : 0.0f;
}

float tanh_mat(float x) {
    // Clamping para evitar overflow
    if (x > LIMITE_EXP_SUPERIOR) return 1.0f;
    if (x < LIMITE_EXP_INFERIOR) return -1.0f;
    return tanhf(x);
}

float tanh_derivada_mat(float x) {
    float t = tanh_mat(x);
    return 1.0f - t * t;
}

void softmax_mat(float *entradas, int n, float *salidas) {
    if (entradas == NULL || salidas == NULL || n <= 0) {
        return;
    }
    
    // Encontrar el máximo para estabilidad numérica
    float max_val = entradas[0];
    for (int i = 1; i < n; i++) {
        if (entradas[i] > max_val) max_val = entradas[i];
    }
    
    // Calcular exponenciales
    float suma = 0.0f;
    for (int i = 0; i < n; i++) {
        salidas[i] = expf(entradas[i] - max_val);
        suma += salidas[i];
    }
    
    // Normalizar
    if (suma > EPSILON) {
        for (int i = 0; i < n; i++) {
            salidas[i] = salidas[i] / suma;
        }
    }
}

// ================================================================
// 2. OPERACIONES VECTORIALES
// ================================================================

float producto_punto_mat(float *a, float *b, int n) {
    if (a == NULL || b == NULL) {
        return 0.0f;
    }
    
    float resultado = 0.0f;
    for (int i = 0; i < n; i++) {
        resultado += a[i] * b[i];
    }
    return resultado;
}

void sumar_vectores_mat(float *a, float *b, int n, float *resultado) {
    if (a == NULL || b == NULL || resultado == NULL) {
        return;
    }
    
    for (int i = 0; i < n; i++) {
        resultado[i] = a[i] + b[i];
    }
}

void restar_vectores_mat(float *a, float *b, int n, float *resultado) {
    if (a == NULL || b == NULL || resultado == NULL) {
        return;
    }
    
    for (int i = 0; i < n; i++) {
        resultado[i] = a[i] - b[i];
    }
}

void multiplicar_escalar_vector_mat(float *vector, int n, float escalar, float *resultado) {
    if (vector == NULL || resultado == NULL) {
        return;
    }
    
    for (int i = 0; i < n; i++) {
        resultado[i] = vector[i] * escalar;
    }
}

float norma_vector_mat(float *vector, int n) {
    if (vector == NULL || n <= 0) {
        return 0.0f;
    }
    
    float suma = 0.0f;
    for (int i = 0; i < n; i++) {
        suma += vector[i] * vector[i];
    }
    return sqrtf(suma);
}

int normalizar_vector_mat(float *vector, int n, float *resultado) {
    if (vector == NULL || resultado == NULL || n <= 0) {
        return 0;
    }
    
    float norma = norma_vector_mat(vector, n);
    if (norma < EPSILON) {
        return 0;  // Vector nulo
    }
    
    for (int i = 0; i < n; i++) {
        resultado[i] = vector[i] / norma;
    }
    return 1;
}

// ================================================================
// 3. OPERACIONES MATRICIALES
// ================================================================

float** crear_matriz_mat(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0) {
        return NULL;
    }
    
    float **matriz = (float**)malloc(filas * sizeof(float*));
    if (matriz == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < filas; i++) {
        matriz[i] = (float*)malloc(columnas * sizeof(float));
        if (matriz[i] == NULL) {
            // Liberar memoria ya asignada
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    
    return matriz;
}

void liberar_matriz_mat(float **matriz, int filas) {
    if (matriz == NULL) {
        return;
    }
    
    for (int i = 0; i < filas; i++) {
        if (matriz[i] != NULL) {
            free(matriz[i]);
            matriz[i] = NULL;
        }
    }
    free(matriz);
    matriz = NULL;
}

int multiplicar_matrices_mat(float **A, int filas_A, int columnas_A,
                              float **B, int filas_B, int columnas_B,
                              float **resultado) {
    // Validaciones
    if (A == NULL || B == NULL || resultado == NULL) {
        return 0;
    }
    
    if (columnas_A != filas_B) {
        return 0;  // Dimensiones incompatibles
    }
    
    // Multiplicación
    for (int i = 0; i < filas_A; i++) {
        for (int j = 0; j < columnas_B; j++) {
            float suma = 0.0f;
            for (int k = 0; k < columnas_A; k++) {
                suma += A[i][k] * B[k][j];
            }
            resultado[i][j] = suma;
        }
    }
    
    return 1;
}

void transponer_matriz_mat(float **matriz, int filas, int columnas, float **resultado) {
    if (matriz == NULL || resultado == NULL) {
        return;
    }
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            resultado[j][i] = matriz[i][j];
        }
    }
}

void imprimir_matriz_mat(float **matriz, int filas, int columnas, const char *nombre) {
    if (matriz == NULL) {
        printf("  Matriz NULL\n");
        return;
    }
    
    printf("\n  === Matriz: %s (%d×%d) ===\n", nombre, filas, columnas);
    for (int i = 0; i < filas; i++) {
        printf("  ");
        for (int j = 0; j < columnas; j++) {
            printf("%8.4f ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("  ============================\n");
}

// ================================================================
// 4. FUNCIONES ESTADÍSTICAS
// ================================================================

float media_vector_mat(float *vector, int n) {
    if (vector == NULL || n <= 0) {
        return 0.0f;
    }
    
    float suma = 0.0f;
    for (int i = 0; i < n; i++) {
        suma += vector[i];
    }
    return suma / n;
}

float varianza_vector_mat(float *vector, int n, float media) {
    if (vector == NULL || n <= 0) {
        return 0.0f;
    }
    
    float suma = 0.0f;
    for (int i = 0; i < n; i++) {
        float diff = vector[i] - media;
        suma += diff * diff;
    }
    return suma / n;
}

float desviacion_vector_mat(float *vector, int n) {
    if (vector == NULL || n <= 0) {
        return 0.0f;
    }
    
    float media = media_vector_mat(vector, n);
    float varianza = varianza_vector_mat(vector, n, media);
    return sqrtf(varianza);
}

float maximo_vector_mat(float *vector, int n) {
    if (vector == NULL || n <= 0) {
        return 0.0f;
    }
    
    float max_val = vector[0];
    for (int i = 1; i < n; i++) {
        if (vector[i] > max_val) max_val = vector[i];
    }
    return max_val;
}

float minimo_vector_mat(float *vector, int n) {
    if (vector == NULL || n <= 0) {
        return 0.0f;
    }
    
    float min_val = vector[0];
    for (int i = 1; i < n; i++) {
        if (vector[i] < min_val) min_val = vector[i];
    }
    return min_val;
}

// ================================================================
// 5. FUNCIONES DE UTILERÍA
// ================================================================

float truncar_decimales_mat(float valor, int decimales) {
    if (decimales < 0) {
        return valor;
    }
    
    float factor = 1.0f;
    for (int i = 0; i < decimales; i++) {
        factor *= 10.0f;
    }
    
    return (float)((int)(valor * factor)) / factor;
}

float clamp_mat(float valor, float min, float max) {
    if (valor < min) return min;
    if (valor > max) return max;
    return valor;
}

int es_numero_valido_mat(float valor) {
    return !isnan(valor) && !isinf(valor);
}

float escalar_xor_mat(float a, float b) {
    // XOR: (a OR b) AND NOT (a AND b)
    float resultado = (a + b) - (2.0f * a * b);
    return clamp_mat(resultado, 0.0f, 1.0f);
}

// ================================================================
// FIN DEL ARCHIVO matematica.c
// ================================================================