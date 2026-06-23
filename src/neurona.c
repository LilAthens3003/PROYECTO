// ================================================================
// neurona.c - Implementación del Módulo de Neurona Artificial
// ================================================================
// 
// DESCRIPCIÓN:
//   Este archivo contiene la implementación completa de todas las
//   funciones declaradas en neurona.h. Incluye:
//   - Creación y destrucción de neurona
//   - Forward y backward propagation (usando módulo matemáticas)
//   - Entrenamiento y predicción
//   - Integración con módulo dataset
//   - Funciones de depuración
//   - Manejo de errores con mensajes claros
//
// DEPENDENCIAS:
//   - neurona.h (declaraciones)
//   - dataset.h (para carga y manipulación de datos)
//   - matematica.h (para funciones matemáticas especializadas)
//
// AUTOR: Alumno 1 - Matemática y Núcleo de IA
// FECHA: 2026
// VERSIÓN: 3.0 (Con matemáticas integradas)
// ================================================================

#include "../include/neurona.h"
#include "../include/dataset.h"     // Para carga y manipulación de datos
#include "../include/matematica.h"  // ★ NUEVO: Para funciones matemáticas

// ================================================================
// 1. FUNCIONES DE CREACIÓN Y DESTRUCCIÓN
// ================================================================

Neurona* crear_neurona(int num_entradas) {
    // Validar que el número de entradas sea válido
    if (num_entradas <= 0) {
        mostrar_error("Error: El número de entradas debe ser mayor a 0.");
        return NULL;
    }
    
    // Reservar memoria para la neurona
    Neurona *n = (Neurona*)malloc(sizeof(Neurona));
    if (n == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para la neurona.");
        return NULL;
    }
    
    // Asignar valores iniciales
    n->num_entradas = num_entradas;
    n->bias = 0.0f;
    n->salida = 0.0f;
    n->error = 0.0f;
    
    // Reservar memoria para los pesos
    n->pesos = (float*)malloc(num_entradas * sizeof(float));
    if (n->pesos == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para los pesos.");
        free(n);
        return NULL;
    }
    
    return n;
}

void liberar_neurona(Neurona *n) {
    if (n != NULL) {
        if (n->pesos != NULL) {
            free(n->pesos);
            n->pesos = NULL;
        }
        free(n);
        n = NULL;
    }
}

void inicializar_pesos_aleatorios(Neurona *n) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula al inicializar pesos.");
        return;
    }
    
    // Inicializar semilla aleatoria una sola vez
    static int semilla_inicializada = 0;
    if (!semilla_inicializada) {
        srand((unsigned int)time(NULL));
        semilla_inicializada = 1;
    }
    
    // Asignar pesos aleatorios entre -1.0 y 1.0
    for (int i = 0; i < n->num_entradas; i++) {
        n->pesos[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    }
    
    // Asignar bias aleatorio entre -1.0 y 1.0
    n->bias = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
}

// ================================================================
// 2. FUNCIONES PRINCIPALES DE LA NEURONA
// ================================================================

float forward_propagation(Neurona *n, float *entradas) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en forward_propagation.");
        return 0.0f;
    }
    
    if (entradas == NULL) {
        mostrar_error("Error: Arreglo de entradas nulo en forward_propagation.");
        return 0.0f;
    }
    
    // 1. Calcular suma ponderada: Σ(entrada × peso)
    // ★ AHORA USA producto_punto_mat() del módulo matemáticas ★
    float suma = producto_punto_mat(entradas, n->pesos, n->num_entradas);
    
    // 2. Agregar el bias
    suma += n->bias;
    
    // 3. Aplicar función de activación Sigmoid
    // ★ AHORA USA sigmoid_mat() del módulo matemáticas ★
    n->salida = sigmoid_mat(suma);
    
    return n->salida;
}

void backward_propagation(Neurona *n, float *entradas, float esperado, float tasa) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en backward_propagation.");
        return;
    }
    
    if (entradas == NULL) {
        mostrar_error("Error: Arreglo de entradas nulo en backward_propagation.");
        return;
    }
    
    // 1. Calcular el error
    n->error = esperado - n->salida;
    
    // 2. Calcular delta: error × derivada de la activación
    // ★ AHORA USA sigmoid_derivada_mat() del módulo matemáticas ★
    float delta = n->error * sigmoid_derivada_mat(n->salida);
    
    // 3. Actualizar cada peso: peso_nuevo = peso_viejo + tasa × delta × entrada
    for (int i = 0; i < n->num_entradas; i++) {
        n->pesos[i] += tasa * delta * entradas[i];
    }
    
    // 4. Actualizar el bias
    n->bias += tasa * delta;
}

// ================================================================
// 3. FUNCIONES DE ENTRENAMIENTO Y PREDICCIÓN
// ================================================================

float* entrenar_neurona(Neurona *n, Dataset *datos, int epocas, float tasa) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en entrenar_neurona.");
        return NULL;
    }
    
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en entrenar_neurona.");
        return NULL;
    }
    
    if (epocas <= 0) {
        mostrar_error("Error: El número de épocas debe ser mayor a 0.");
        return NULL;
    }
    
    // ★ NUEVO: Validar el dataset antes de entrenar ★
    if (!validar_dataset(datos)) {
        mostrar_error("Error: Dataset inválido en entrenar_neurona.");
        return NULL;
    }
    
    // Reservar memoria para el historial de errores
    float *historial = (float*)malloc(epocas * sizeof(float));
    if (historial == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para el historial de errores.");
        return NULL;
    }
    
    // Entrenamiento por épocas
    for (int epoca = 0; epoca < epocas; epoca++) {
        float error_total = 0.0f;
        
        // Una época = recorrer todo el dataset
        for (int i = 0; i < datos->num_muestras; i++) {
            // Forward propagation
            float salida = forward_propagation(n, datos->entradas[i]);
            
            // Backward propagation
            backward_propagation(n, datos->entradas[i], datos->salidas[i], tasa);
            
            // Acumular error al cuadrado
            error_total += n->error * n->error;
        }
        
        // Error promedio de la época
        float error_epoca = error_total / datos->num_muestras;
        historial[epoca] = error_epoca;
        
        // ★ DEPURACIÓN: Mostrar progreso cada 10% ★
        if (epocas >= 10 && (epoca + 1) % (epocas / 10) == 0) {
            printf("  [Depuración] Época %d/%d - Error: %.6f\n", 
                   epoca + 1, epocas, error_epoca);
        }
    }
    
    return historial;
}

/**
 * entrenar_neurona_con_config - Entrena la neurona usando configuración
 * 
 * ★ NUEVA FUNCIÓN: Simplifica el entrenamiento desde el menú ★
 * 
 * PARÁMETROS:
 *   n      - Puntero a la neurona
 *   config - Puntero a la configuración
 * 
 * RETORNO:
 *   Arreglo con historial de errores, o NULL si falla
 */
float* entrenar_neurona_con_config(Neurona *n, Configuracion *config) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en entrenar_neurona_con_config.");
        return NULL;
    }
    
    if (config == NULL) {
        mostrar_error("Error: Configuración nula en entrenar_neurona_con_config.");
        return NULL;
    }
    
    // Cargar dataset usando la configuración
    Dataset *datos = cargar_dataset_desde_config(config);
    if (datos == NULL) {
        mostrar_error("Error: No se pudo cargar el dataset para entrenar.");
        return NULL;
    }
    
    // ★ MODIFICAR AQUÍ si los datos ya están normalizados ★
    // Normalizar los datos (asumiendo que vienen de una imagen 0-255)
    normalizar_dataset(datos, 255.0f);
    
    // Entrenar la neurona
    float *historial = entrenar_neurona(n, datos, config->epocas, config->tasa_aprendizaje);
    
    // Liberar el dataset (ya no lo necesitamos)
    liberar_dataset(datos);
    
    return historial;
}

float predecir(Neurona *n, float *entradas) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en predecir.");
        return 0.0f;
    }
    
    return forward_propagation(n, entradas);
}

/**
 * predecir_con_validacion - Predicción con validación de entrada
 * 
 * ★ NUEVA FUNCIÓN: Seguridad en predicciones ★
 * 
 * PARÁMETROS:
 *   n        - Puntero a la neurona (ya entrenada)
 *   entradas - Arreglo de valores de entrada
 *   num_entradas - Número de entradas (para validación)
 * 
 * RETORNO:
 *   Salida de la neurona (entre 0 y 1), o -1.0f si falla
 */
float predecir_con_validacion(Neurona *n, float *entradas, int num_entradas) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en predecir_con_validacion.");
        return -1.0f;
    }
    
    if (entradas == NULL) {
        mostrar_error("Error: Arreglo de entradas nulo en predecir_con_validacion.");
        return -1.0f;
    }
    
    if (num_entradas != n->num_entradas) {
        char mensaje[256];
        sprintf(mensaje, "Error: Número de entradas incorrecto. Esperadas: %d, Recibidas: %d",
                n->num_entradas, num_entradas);
        mostrar_error(mensaje);
        return -1.0f;
    }
    
    return forward_propagation(n, entradas);
}

int clasificar(float salida) {
    return (salida >= UMBRAL_CLASIFICACION) ? 1 : 0;
}

// ================================================================
// 4. FUNCIONES DE CONFIGURACIÓN
// ================================================================

Configuracion* leer_configuracion(const char *nombre_archivo) {
    if (nombre_archivo == NULL) {
        mostrar_error("Error: Nombre de archivo nulo en leer_configuracion.");
        return NULL;
    }
    
    FILE *f = fopen(nombre_archivo, "r");
    if (f == NULL) {
        mostrar_error("Error: No se pudo abrir el archivo de configuración.");
        return NULL;
    }
    
    Configuracion *config = (Configuracion*)malloc(sizeof(Configuracion));
    if (config == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para la configuración.");
        fclose(f);
        return NULL;
    }
    
    // Valores por defecto
    config->num_entradas = 25;
    config->tasa_aprendizaje = TASA_APRENDIZAJE_DEFECTO;
    config->epocas = EPOCAS_DEFECTO;
    strcpy(config->archivo_datos, "dataset.csv");
    
    char linea[256];
    char clave[50];
    char valor[50];
    
    while (fgets(linea, sizeof(linea), f)) {
        // Ignorar comentarios y líneas vacías
        if (linea[0] == '#' || linea[0] == '\n' || linea[0] == '\r') {
            continue;
        }
        
        // Eliminar salto de línea
        size_t len = strlen(linea);
        if (len > 0 && (linea[len-1] == '\n' || linea[len-1] == '\r')) {
            linea[len-1] = '\0';
        }
        if (len > 1 && linea[len-2] == '\r') {
            linea[len-2] = '\0';
        }
        
        if (sscanf(linea, "%[^=]=%s", clave, valor) == 2) {
            // Eliminar espacios en blanco
            char *p;
            while ((p = strchr(clave, ' '))) *p = '\0';
            while ((p = strchr(valor, ' '))) *p = '\0';
            
            if (strcmp(clave, "NUM_ENTRADAS") == 0) {
                config->num_entradas = atoi(valor);
                if (config->num_entradas <= 0) {
                    mostrar_error("Error: NUM_ENTRADAS debe ser mayor a 0.");
                    free(config);
                    fclose(f);
                    return NULL;
                }
            }
            else if (strcmp(clave, "TASA_APRENDIZAJE") == 0) {
                config->tasa_aprendizaje = (float)atof(valor);
                if (config->tasa_aprendizaje <= 0.0f || config->tasa_aprendizaje > 1.0f) {
                    mostrar_error("Error: TASA_APRENDIZAJE debe estar entre 0 y 1.");
                    free(config);
                    fclose(f);
                    return NULL;
                }
            }
            else if (strcmp(clave, "EPOCAS") == 0) {
                config->epocas = atoi(valor);
                if (config->epocas <= 0) {
                    mostrar_error("Error: EPOCAS debe ser mayor a 0.");
                    free(config);
                    fclose(f);
                    return NULL;
                }
            }
            else if (strcmp(clave, "ARCHIVO_DATOS") == 0) {
                strcpy(config->archivo_datos, valor);
            }
        }
    }
    
    fclose(f);
    
    // ★ DEPURACIÓN: Mostrar configuración cargada ★
    printf("  [Depuración] Configuración cargada:\n");
    printf("    - Num_entradas: %d\n", config->num_entradas);
    printf("    - Tasa aprendizaje: %.3f\n", config->tasa_aprendizaje);
    printf("    - Épocas: %d\n", config->epocas);
    printf("    - Archivo datos: %s\n", config->archivo_datos);
    
    return config;
}

void liberar_configuracion(Configuracion *config) {
    if (config != NULL) {
        free(config);
        config = NULL;
    }
}

// ================================================================
// 5. FUNCIONES DE DEPURACIÓN Y VISUALIZACIÓN
// ================================================================

void imprimir_estado_neurona(Neurona *n) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en imprimir_estado_neurona.");
        return;
    }
    
    printf("\n=== ESTADO DE LA NEURONA ===\n");
    printf("  Número de entradas: %d\n", n->num_entradas);
    printf("  Bias: %.6f\n", n->bias);
    printf("  Última salida: %.6f\n", n->salida);
    printf("  Último error: %.6f\n", n->error);
    printf("==============================\n\n");
}

void imprimir_pesos(Neurona *n) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en imprimir_pesos.");
        return;
    }
    
    printf("\n=== PESOS DE LA NEURONA ===\n");
    printf("  Bias: %.6f\n", n->bias);
    printf("  Pesos:\n");
    for (int i = 0; i < n->num_entradas; i++) {
        printf("    [%3d] = %.6f\n", i, n->pesos[i]);
    }
    printf("==============================\n\n");
}

void imprimir_error_historial(float *errores, int num_errores, int mostrar_todos) {
    if (errores == NULL) {
        mostrar_error("Error: Arreglo de errores nulo en imprimir_error_historial.");
        return;
    }
    
    if (num_errores <= 0) {
        mostrar_error("Error: Número de errores inválido.");
        return;
    }
    
    printf("\n=== HISTORIAL DE ERRORES ===\n");
    printf("  Total de épocas: %d\n", num_errores);
    printf("  Error inicial: %.6f\n", errores[0]);
    printf("  Error final: %.6f\n", errores[num_errores - 1]);
    
    if (mostrar_todos) {
        printf("  Detalle:\n");
        for (int i = 0; i < num_errores; i++) {
            printf("    Época %d: %.6f\n", i + 1, errores[i]);
        }
    } else {
        // Mostrar resumen cada 10%
        printf("  Resumen (cada 10%%):\n");
        int paso = num_errores / 10;
        if (paso < 1) paso = 1;
        for (int i = 0; i < num_errores; i += paso) {
            printf("    Época %d: %.6f\n", i + 1, errores[i]);
        }
    }
    printf("==============================\n\n");
}

int guardar_pesos_archivo(Neurona *n, const char *nombre_archivo) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en guardar_pesos_archivo.");
        return 0;
    }
    
    if (nombre_archivo == NULL) {
        mostrar_error("Error: Nombre de archivo nulo en guardar_pesos_archivo.");
        return 0;
    }
    
    FILE *f = fopen(nombre_archivo, "w");
    if (f == NULL) {
        char mensaje[256];
        sprintf(mensaje, "Error: No se pudo abrir el archivo '%s' para guardar.", 
                nombre_archivo);
        mostrar_error(mensaje);
        return 0;
    }
    
    // Escribir cabecera
    fprintf(f, "# Pesos entrenados de SINA-VISUAL\n");
    fprintf(f, "# Fecha: %s\n", __DATE__);
    fprintf(f, "# Hora: %s\n", __TIME__);
    fprintf(f, "# Número de entradas: %d\n", n->num_entradas);
    fprintf(f, "BIAS=%.6f\n", n->bias);
    
    // Escribir pesos
    fprintf(f, "PESOS=");
    for (int i = 0; i < n->num_entradas; i++) {
        fprintf(f, "%.6f", n->pesos[i]);
        if (i < n->num_entradas - 1) {
            fprintf(f, ",");
        }
    }
    fprintf(f, "\n");
    
    fclose(f);
    return 1;
}

// ================================================================
// 6. FUNCIONES DE CARGA DE PESOS (NUEVAS)
// ================================================================

/**
 * cargar_pesos_archivo - Carga pesos desde un archivo de texto
 * 
 * ★ NUEVA FUNCIÓN: Carga pesos previamente guardados ★
 * 
 * PARÁMETROS:
 *   n            - Puntero a la neurona (ya creada)
 *   nombre_archivo - Nombre del archivo a cargar
 * 
 * RETORNO:
 *   1 si se cargó correctamente, 0 si falló
 */
int cargar_pesos_archivo(Neurona *n, const char *nombre_archivo) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en cargar_pesos_archivo.");
        return 0;
    }
    
    if (nombre_archivo == NULL) {
        mostrar_error("Error: Nombre de archivo nulo en cargar_pesos_archivo.");
        return 0;
    }
    
    FILE *f = fopen(nombre_archivo, "r");
    if (f == NULL) {
        char mensaje[256];
        sprintf(mensaje, "Error: No se pudo abrir el archivo '%s' para cargar.",
                nombre_archivo);
        mostrar_error(mensaje);
        return 0;
    }
    
    char linea[256];
    int pesos_cargados = 0;
    int bias_cargado = 0;
    
    while (fgets(linea, sizeof(linea), f)) {
        // Ignorar comentarios
        if (linea[0] == '#') continue;
        
        // Eliminar salto de línea
        size_t len = strlen(linea);
        if (len > 0 && (linea[len-1] == '\n' || linea[len-1] == '\r')) {
            linea[len-1] = '\0';
        }
        
        // Buscar BIAS
        if (strncmp(linea, "BIAS=", 5) == 0) {
            float bias;
            if (sscanf(linea + 5, "%f", &bias) == 1) {
                n->bias = bias;
                bias_cargado = 1;
                printf("  [Depuración] Bias cargado: %.6f\n", bias);
            }
        }
        
        // Buscar PESOS
        if (strncmp(linea, "PESOS=", 6) == 0) {
            char *p = linea + 6;
            int idx = 0;
            char *token = strtok(p, ",");
            
            while (token != NULL && idx < n->num_entradas) {
                float peso;
                if (sscanf(token, "%f", &peso) == 1) {
                    n->pesos[idx] = peso;
                    idx++;
                }
                token = strtok(NULL, ",");
            }
            
            if (idx == n->num_entradas) {
                pesos_cargados = 1;
                printf("  [Depuración] %d pesos cargados.\n", idx);
            } else {
                char mensaje[256];
                sprintf(mensaje, "Error: Se esperaban %d pesos, se cargaron %d.",
                        n->num_entradas, idx);
                mostrar_error(mensaje);
            }
        }
    }
    
    fclose(f);
    
    if (pesos_cargados && bias_cargado) {
        printf("  [Depuración] Pesos y bias cargados correctamente.\n");
        return 1;
    } else {
        mostrar_error("Error: No se pudieron cargar todos los datos del archivo.");
        return 0;
    }
}

// ================================================================
// 7. FUNCIONES DE MANEJO DE ERRORES
// ================================================================

void mostrar_error(const char *mensaje) {
    if (mensaje == NULL) return;
    
    // Usar printf para consola
    printf("\n❌ %s\n", mensaje);
    
    // ★ Si se usa raylib, aquí se puede mostrar en pantalla ★
    // Ejemplo: DrawText(mensaje, 10, 10, 20, RED);
}

// ================================================================
// FIN DEL ARCHIVO neurona.c
// ================================================================