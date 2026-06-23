/* ================================================================
   neurona.c - Implementación del Módulo de Neurona Artificial
   ================================================================
   
   DESCRIPCIÓN:
     Este archivo contiene la implementación completa de todas las
     funciones declaradas en neurona.h. Incluye:
     - Creación y destrucción de neurona
     - Forward y backward propagation
     - Carga y validación de datasets desde CSV
     - Normalización de datos
     - Funciones de depuración
     - Manejo de errores con mensajes claros
   
   AUTOR: Alumno 1 - Matemática y Núcleo de IA
   FECHA: 2026
   VERSIÓN: 1.0
   ================================================================ */

#include "../include/neurona.h"

/* ================================================================
 MODIFICAR AQUÍ PARA CAMBIAR PRECISIÓN
 - 100.0f  → 2 decimales  (RECOMENDADO)
 - 1000.0f → 3 decimales
 - 10000.0f → 4 decimales
 ================================================================*/
#define PRECISION_DECIMAL 100.0f

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
// 2. FUNCIONES MATEMÁTICAS Y DE ACTIVACIÓN
// ================================================================

float sigmoid(float x) {
    // Clamping para evitar overflow en exp()
    if (x > 50.0f) return 1.0f;
    if (x < -50.0f) return 0.0f;
    return 1.0f / (1.0f + expf(-x));
}

float sigmoid_derivada(float x) {
    float s = sigmoid(x);
    return s * (1.0f - s);
}

float producto_punto(float *a, float *b, int n) {
    if (a == NULL || b == NULL) {
        mostrar_error("Error: Vector nulo en producto_punto.");
        return 0.0f;
    }
    
    float resultado = 0.0f;
    for (int i = 0; i < n; i++) {
        resultado += a[i] * b[i];
    }
    return resultado;
}

// ================================================================
// 3. FUNCIONES PRINCIPALES DE LA NEURONA
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
    float suma = producto_punto(entradas, n->pesos, n->num_entradas);
    
    // 2. Agregar el bias
    suma += n->bias;
    
    // 3. Aplicar función de activación Sigmoid
    n->salida = sigmoid(suma);
    
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
    float delta = n->error * sigmoid_derivada(n->salida);
    
    // 3. Actualizar cada peso: peso_nuevo = peso_viejo + tasa × delta × entrada
    for (int i = 0; i < n->num_entradas; i++) {
        n->pesos[i] += tasa * delta * entradas[i];
    }
    
    // 4. Actualizar el bias
    n->bias += tasa * delta;
}

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

float predecir(Neurona *n, float *entradas) {
    if (n == NULL) {
        mostrar_error("Error: Neurona nula en predecir.");
        return 0.0f;
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
// 5. FUNCIONES DE DATASET - CON VALIDACIÓN COMPLETA
// ================================================================

/*validar_y_convertir_numero - Valida que una cadena sea un número válido
  
   VALIDACIÓN COMPLETA
  - Rechaza cadenas vacías
  - Rechaza caracteres no numéricos (letras, símbolos)
  - Acepta números con punto decimal
  - Trunca a 2 decimales (PRECISION_DECIMAL) para controlar precisión
  
  PARÁMETROS:
    cadena - Texto a validar
    valor  - Puntero donde guardar el número convertido
  
  RETORNO:
    1 si es número válido, 0 si no*/
int validar_y_convertir_numero(const char *cadena, float *valor) {
    // Validar que la cadena no sea nula o vacía
    if (cadena == NULL || cadena[0] == '\0') {
        return 0;
    }
    
    // Verificar que solo tenga caracteres numéricos, punto y signo negativo
    int tiene_punto = 0;
    int tiene_signo = 0;
    int tiene_digito = 0;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        char c = cadena[i];
        
        // Permitir espacio al inicio (lo ignoramos)
        if (i == 0 && c == ' ') continue;
        
        // Permitir signo negativo solo al inicio
        if (c == '-' && i == 0 && !tiene_signo) {
            tiene_signo = 1;
            continue;
        }
        
        // Permitir punto decimal (solo uno)
        if (c == '.' && !tiene_punto) {
            tiene_punto = 1;
            continue;
        }
        
        // Verificar que sea un dígito
        if (c >= '0' && c <= '9') {
            tiene_digito = 1;
            continue;
        }
        
        // Si llegamos aquí, el carácter no es válido
        return 0;
    }
    
    // Debe tener al menos un dígito
    if (!tiene_digito) {
        return 0;
    }
    
    // Convertir la cadena a número
    char *endptr;
    float temp = strtof(cadena, &endptr);
    
    // Verificar que la conversión fue exitosa
    if (endptr == cadena || *endptr != '\0') {
        return 0;
    }
    
    // ★ TRUNCAR A 2 DECIMALES (MODIFICABLE) ★
    // Suficiente para el aprendizaje de la neurona
    // ★ MODIFICAR PRECISION_DECIMAL para cambiar precisión ★
    *valor = (float)((int)(temp * PRECISION_DECIMAL)) / PRECISION_DECIMAL;
    
    return 1;
}

Dataset* cargar_dataset(const char *nombre_archivo) {
    if (nombre_archivo == NULL) {
        mostrar_error("Error: Nombre de archivo nulo en cargar_dataset.");
        return NULL;
    }
    
    FILE *f = fopen(nombre_archivo, "r");
    if (f == NULL) {
        char mensaje[256];
        sprintf(mensaje, "Error: No se pudo abrir el archivo '%s'.", nombre_archivo);
        mostrar_error(mensaje);
        return NULL;
    }
    
    // PRIMERA PASADA: Contar líneas y características
    int num_muestras = 0;
    int num_caracteristicas = 0;
    char linea[1024];
    
    while (fgets(linea, sizeof(linea), f)) {
        // Ignorar líneas vacías o con solo comentarios
        int es_comentario = 0;
        for (int i = 0; linea[i] != '\0'; i++) {
            if (linea[i] == '#' || linea[i] == '\n' || linea[i] == '\r') {
                break;
            }
            if (linea[i] != ' ' && linea[i] != '\t') {
                es_comentario = 1;
                break;
            }
        }
        if (!es_comentario) continue;
        
        if (num_muestras == 0) {
            // Contar columnas en la primera línea válida
            num_caracteristicas = contar_columnas_csv(linea);
            if (num_caracteristicas <= 1) {
                mostrar_error("Error: El CSV debe tener al menos 2 columnas (entradas + salida).");
                fclose(f);
                return NULL;
            }
            // Restar 1 porque la última columna es la salida esperada
            num_caracteristicas--;
        }
        num_muestras++;
    }
    
    if (num_muestras == 0) {
        mostrar_error("Error: El archivo CSV está vacío o no contiene datos válidos.");
        fclose(f);
        return NULL;
    }
    
    // Reservar memoria para el dataset
    Dataset *datos = (Dataset*)malloc(sizeof(Dataset));
    if (datos == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para el dataset.");
        fclose(f);
        return NULL;
    }
    
    datos->num_muestras = num_muestras;
    datos->num_caracteristicas = num_caracteristicas;
    datos->entradas = (float**)malloc(num_muestras * sizeof(float*));
    datos->salidas = (float*)malloc(num_muestras * sizeof(float));
    
    if (datos->entradas == NULL || datos->salidas == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para los datos.");
        free(datos);
        fclose(f);
        return NULL;
    }
    
    for (int i = 0; i < num_muestras; i++) {
        datos->entradas[i] = (float*)malloc(num_caracteristicas * sizeof(float));
        if (datos->entradas[i] == NULL) {
            mostrar_error("Error: No se pudo asignar memoria para una fila del dataset.");
            // Limpiar memoria ya asignada
            for (int j = 0; j < i; j++) {
                free(datos->entradas[j]);
            }
            free(datos->entradas);
            free(datos->salidas);
            free(datos);
            fclose(f);
            return NULL;
        }
    }
    
    // SEGUNDA PASADA: Leer los datos con validación
    rewind(f);
    int muestra_actual = 0;
    int linea_actual = 0;
    int errores_encontrados = 0;
    
    while (fgets(linea, sizeof(linea), f)) {
        linea_actual++;
        
        // Ignorar líneas vacías o con solo comentarios
        int es_comentario = 0;
        for (int i = 0; linea[i] != '\0'; i++) {
            if (linea[i] == '#' || linea[i] == '\n' || linea[i] == '\r') {
                break;
            }
            if (linea[i] != ' ' && linea[i] != '\t') {
                es_comentario = 1;
                break;
            }
        }
        if (!es_comentario) continue;
        
        // Eliminar salto de línea
        size_t len = strlen(linea);
        if (len > 0 && (linea[len-1] == '\n' || linea[len-1] == '\r')) {
            linea[len-1] = '\0';
        }
        if (len > 1 && linea[len-2] == '\r') {
            linea[len-2] = '\0';
        }
        
        // Dividir la línea por el separador
        char *tokens[100];
        int num_tokens = 0;
        char *token = strtok(linea, ";");
        while (token != NULL && num_tokens < 100) {
            // Eliminar espacios al inicio y final
            while (*token == ' ' || *token == '\t') token++;
            char *end = token + strlen(token) - 1;
            while (end > token && (*end == ' ' || *end == '\t')) end--;
            *(end + 1) = '\0';
            
            tokens[num_tokens++] = token;
            token = strtok(NULL, ";");
        }
        
        // Verificar que tenga el número correcto de columnas
        int columnas_esperadas = num_caracteristicas + 1;
        if (num_tokens != columnas_esperadas) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d tiene %d columnas, se esperaban %d.",
                    linea_actual, num_tokens, columnas_esperadas);
            mostrar_error(mensaje);
            errores_encontrados++;
            continue;
        }
        
        // Validar y convertir cada valor
        int columna_actual = 0;
        int error_fila = 0;
        
        // Leer entradas
        for (int j = 0; j < num_caracteristicas; j++) {
            float valor;
            if (!validar_y_convertir_numero(tokens[j], &valor)) {
                char mensaje[256];
                sprintf(mensaje, "Error: Línea %d, columna %d: '%s' no es un número válido.",
                        linea_actual, j + 1, tokens[j]);
                mostrar_error(mensaje);
                errores_encontrados++;
                error_fila = 1;
                break;
            }
            datos->entradas[muestra_actual][j] = valor;
        }
        
        if (error_fila) continue;
        
        // Leer salida esperada (última columna)
        float salida;
        if (!validar_y_convertir_numero(tokens[num_tokens - 1], &salida)) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d, última columna: '%s' no es un número válido.",
                    linea_actual, tokens[num_tokens - 1]);
            mostrar_error(mensaje);
            errores_encontrados++;
            continue;
        }
        datos->salidas[muestra_actual] = salida;
        
        muestra_actual++;
    }
    
    fclose(f);
    
    // Verificar si hubo errores
    if (errores_encontrados > 0) {
        char mensaje[256];
        sprintf(mensaje, "Error: Se encontraron %d errores en el archivo CSV.", 
                errores_encontrados);
        mostrar_error(mensaje);
        liberar_dataset(datos);
        return NULL;
    }
    
    // Verificar que se leyeron todas las muestras
    if (muestra_actual != num_muestras) {
        mostrar_error("Error: No se pudieron leer todas las muestras del CSV.");
        liberar_dataset(datos);
        return NULL;
    }
    
    // ★ DEPURACIÓN: Mostrar información del dataset ★
    printf("  [Depuración] Dataset cargado exitosamente:\n");
    printf("    - Muestras: %d\n", datos->num_muestras);
    printf("    - Características: %d\n", datos->num_caracteristicas);
    printf("  [Depuración] Primeras 3 muestras:\n");
    for (int i = 0; i < 3 && i < datos->num_muestras; i++) {
        printf("    Muestra %d: ", i + 1);
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            printf("%.2f ", datos->entradas[i][j]);
        }
        printf("→ %.2f\n", datos->salidas[i]);
    }
    
    return datos;
}

void liberar_dataset(Dataset *datos) {
    if (datos != NULL) {
        if (datos->entradas != NULL) {
            for (int i = 0; i < datos->num_muestras; i++) {
                if (datos->entradas[i] != NULL) {
                    free(datos->entradas[i]);
                    datos->entradas[i] = NULL;
                }
            }
            free(datos->entradas);
            datos->entradas = NULL;
        }
        if (datos->salidas != NULL) {
            free(datos->salidas);
            datos->salidas = NULL;
        }
        free(datos);
        datos = NULL;
    }
}

void normalizar_dataset(Dataset *datos, float max_valor) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en normalizar_dataset.");
        return;
    }
    
    if (max_valor <= 0.0f) {
        mostrar_error("Error: max_valor debe ser mayor a 0 en normalizar_dataset.");
        return;
    }
    
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            datos->entradas[i][j] = datos->entradas[i][j] / max_valor;
            // Asegurar que esté en el rango [0, 1]
            if (datos->entradas[i][j] < 0.0f) datos->entradas[i][j] = 0.0f;
            if (datos->entradas[i][j] > 1.0f) datos->entradas[i][j] = 1.0f;
        }
    }
}

// ================================================================
// 6. FUNCIONES DE DEPURACIÓN Y VISUALIZACIÓN
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
// 8. FUNCIONES DE AYUDA Y UTILERÍA
// ================================================================

int contar_lineas_archivo(FILE *archivo) {
    if (archivo == NULL) {
        mostrar_error("Error: Archivo nulo en contar_lineas_archivo.");
        return 0;
    }
    
    int lineas = 0;
    char buffer[1024];
    long pos = ftell(archivo);
    
    rewind(archivo);
    while (fgets(buffer, sizeof(buffer), archivo)) {
        // Ignorar líneas vacías o con solo comentarios
        int es_valida = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] == '#' || buffer[i] == '\n' || buffer[i] == '\r') {
                break;
            }
            if (buffer[i] != ' ' && buffer[i] != '\t') {
                es_valida = 1;
                break;
            }
        }
        if (es_valida) lineas++;
    }
    
    fseek(archivo, pos, SEEK_SET);
    return lineas;
}

int contar_columnas_csv(const char *linea) {
    if (linea == NULL) return 0;
    
    int columnas = 0;
    int en_campo = 0;
    
    for (int i = 0; linea[i] != '\0' && linea[i] != '\n' && linea[i] != '\r'; i++) {
        if (linea[i] == SEPARADOR_CSV) {
            if (en_campo) {
                columnas++;
                en_campo = 0;
            }
        } else if (linea[i] != ' ' && linea[i] != '\t') {
            if (!en_campo) {
                en_campo = 1;
            }
        }
    }
    if (en_campo) columnas++;
    
    return columnas;
}

// ================================================================
// FIN DEL ARCHIVO neurona.c
// ================================================================