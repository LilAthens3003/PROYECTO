// ================================================================
// dataset.c - Implementación del Módulo de Dataset
// ================================================================
// 
// DESCRIPCIÓN:
//   Este archivo contiene la implementación completa de todas las
//   funciones declaradas en dataset.h. Incluye:
//   - Carga de archivos CSV con validación completa
//   - Normalización y desnormalización de datos
//   - Funciones de consulta y manipulación
//   - Funciones de depuración y visualización
//   - Manejo seguro de memoria dinámica
//
// AUTOR: Alumno 1 - Matemática y Núcleo de IA
// FECHA: 2026
// VERSIÓN: 1.0
// ================================================================

#include "../include/dataset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ================================================================
// 1. FUNCIONES AUXILIARES INTERNAS (PRIVADAS)
// ================================================================

/**
 * es_linea_valida - Verifica si una línea contiene datos (no es comentario)
 * 
 * PARÁMETROS:
 *   linea - Línea de texto a verificar
 * 
 * RETORNO:
 *   1 si la línea contiene datos, 0 si es comentario o vacía
 */
static int es_linea_valida(const char *linea) {
    if (linea == NULL) return 0;
    
    for (int i = 0; linea[i] != '\0'; i++) {
        char c = linea[i];
        // Si encuentra '#', el resto es comentario
        if (c == '#') return 0;
        // Si encuentra un carácter que no es espacio en blanco
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            return 1;
        }
    }
    return 0;  // Línea vacía
}

/**
 * validar_y_convertir_numero_ds - Valida y convierte un número
 * 
 * VALIDACIONES:
 *   - Solo caracteres numéricos, punto y signo negativo
 *   - Un solo punto decimal
 *   - Al menos un dígito
 *   - Sin caracteres especiales o letras
 * 
 * PARÁMETROS:
 *   cadena - Texto a validar
 *   valor  - Puntero donde guardar el número convertido
 * 
 * RETORNO:
 *   1 si es válido, 0 si no
 */
static int validar_y_convertir_numero_ds(const char *cadena, float *valor) {
    // Verificar que la cadena no sea nula o vacía
    if (cadena == NULL || cadena[0] == '\0') {
        return 0;
    }
    
    // Limpiar espacios al inicio y final
    const char *inicio = cadena;
    while (*inicio == ' ' || *inicio == '\t') inicio++;
    
    if (*inicio == '\0') return 0;  // Solo espacios
    
    // Verificar caracteres válidos
    int tiene_punto = 0;
    int tiene_signo = 0;
    int tiene_digito = 0;
    int i = 0;
    
    for (const char *p = inicio; *p != '\0'; p++, i++) {
        char c = *p;
        
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
    
    // Convertir la cadena a número usando strtof
    char *endptr;
    float temp = strtof(inicio, &endptr);
    
    // Verificar que toda la cadena se convirtió
    if (endptr == inicio || *endptr != '\0') {
        // Ignorar espacios finales
        while (*endptr == ' ' || *endptr == '\t') endptr++;
        if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') {
            return 0;
        }
    }
    
    // ★ TRUNCAR A 2 DECIMALES (MODIFICABLE) ★
    // ★ MODIFICAR PRECISION_DECIMAL_DS para cambiar precisión ★
    #define PRECISION_DECIMAL_DS 100.0f
    *valor = (float)((int)(temp * PRECISION_DECIMAL_DS)) / PRECISION_DECIMAL_DS;
    #undef PRECISION_DECIMAL_DS
    
    return 1;
}

/**
 * contar_columnas_csv_ds - Cuenta columnas en una línea CSV
 * 
 * PARÁMETROS:
 *   linea - Línea de texto del archivo CSV
 * 
 * RETORNO:
 *   Número de columnas (valores separados por SEPARADOR_CSV_DATASET)
 */
static int contar_columnas_csv_ds(const char *linea) {
    if (linea == NULL) return 0;
    
    int columnas = 0;
    int en_campo = 0;
    
    for (int i = 0; linea[i] != '\0' && linea[i] != '\n' && linea[i] != '\r'; i++) {
        if (linea[i] == SEPARADOR_CSV_DATASET) {
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
// 2. FUNCIONES DE CARGA DE DATASET
// ================================================================

Dataset* cargar_dataset(const char *nombre_archivo) {
    // === VALIDACIÓN: Archivo no nulo ===
    if (nombre_archivo == NULL) {
        mostrar_error("Error: Nombre de archivo nulo en cargar_dataset.");
        return NULL;
    }
    
    // === VALIDACIÓN: Abrir archivo ===
    FILE *f = fopen(nombre_archivo, "r");
    if (f == NULL) {
        char mensaje[256];
        sprintf(mensaje, "Error: No se pudo abrir el archivo '%s'.", nombre_archivo);
        mostrar_error(mensaje);
        return NULL;
    }
    
    // === PRIMERA PASADA: Contar líneas y características ===
    int num_muestras = 0;
    int num_caracteristicas = -1;  // -1 = no determinado aún
    char linea[MAX_LINEA_CSV];
    int errores_validacion = 0;
    int numero_linea = 0;
    
    while (fgets(linea, sizeof(linea), f)) {
        numero_linea++;
        
        // Ignorar líneas vacías o con comentarios
        if (!es_linea_valida(linea)) continue;
        
        // Contar columnas en esta línea
        int columnas = contar_columnas_csv_ds(linea);
        
        // Validar que tenga al menos 2 columnas (entradas + salida)
        if (columnas < 2) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d tiene %d columna(s), se necesitan al menos 2.",
                    numero_linea, columnas);
            mostrar_error(mensaje);
            errores_validacion++;
            continue;
        }
        
        // Si es la primera línea válida, establecer el número de características
        if (num_caracteristicas == -1) {
            num_caracteristicas = columnas - 1;  // Restar 1 por la salida
        }
        
        // Validar consistencia de columnas
        if (columnas != num_caracteristicas + 1) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d tiene %d columnas, se esperaban %d.",
                    numero_linea, columnas, num_caracteristicas + 1);
            mostrar_error(mensaje);
            errores_validacion++;
            continue;
        }
        
        num_muestras++;
    }
    
    // === VALIDACIÓN: Dataset vacío ===
    if (num_muestras == 0) {
        mostrar_error("Error: El archivo CSV está vacío o no contiene datos válidos.");
        fclose(f);
        return NULL;
    }
    
    // === VALIDACIÓN: Errores en el archivo ===
    if (errores_validacion > 0) {
        char mensaje[256];
        sprintf(mensaje, "Error: Se encontraron %d errores en el archivo CSV.",
                errores_validacion);
        mostrar_error(mensaje);
        fclose(f);
        return NULL;
    }
    
    // === ASIGNAR MEMORIA PARA EL DATASET ===
    Dataset *datos = (Dataset*)malloc(sizeof(Dataset));
    if (datos == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para el dataset.");
        fclose(f);
        return NULL;
    }
    
    datos->num_muestras = num_muestras;
    datos->num_caracteristicas = num_caracteristicas;
    
    // Asignar matriz de entradas
    datos->entradas = (float**)malloc(num_muestras * sizeof(float*));
    if (datos->entradas == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para la matriz de entradas.");
        free(datos);
        fclose(f);
        return NULL;
    }
    
    // Asignar vector de salidas
    datos->salidas = (float*)malloc(num_muestras * sizeof(float));
    if (datos->salidas == NULL) {
        mostrar_error("Error: No se pudo asignar memoria para el vector de salidas.");
        free(datos->entradas);
        free(datos);
        fclose(f);
        return NULL;
    }
    
    // Asignar memoria para cada fila de entradas
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
    
    // === SEGUNDA PASADA: Leer y validar los datos ===
    rewind(f);
    int muestra_actual = 0;
    numero_linea = 0;
    errores_validacion = 0;
    
    while (fgets(linea, sizeof(linea), f)) {
        numero_linea++;
        
        if (!es_linea_valida(linea)) continue;
        
        // Eliminar salto de línea
        size_t len = strlen(linea);
        if (len > 0 && (linea[len-1] == '\n' || linea[len-1] == '\r')) {
            linea[len-1] = '\0';
        }
        if (len > 1 && linea[len-2] == '\r') {
            linea[len-2] = '\0';
        }
        
        // Dividir la línea por el separador
        char separador[2] = { SEPARADOR_CSV_DATASET, '\0' };
        char *tokens[100];
        int num_tokens = 0;
        char *token = strtok(linea, separador);
        
        while (token != NULL && num_tokens < 100) {
            // Eliminar espacios al inicio y final
            while (*token == ' ' || *token == '\t') token++;
            char *end = token + strlen(token) - 1;
            while (end > token && (*end == ' ' || *end == '\t')) end--;
            *(end + 1) = '\0';
            
            tokens[num_tokens++] = token;
            token = strtok(NULL, separador);
        }

        if (token != NULL) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d tiene más de 100 columnas.", numero_linea);
            mostrar_error(mensaje);
            errores_validacion++;
            continue;
        }
        
        // Verificar número de columnas
        int columnas_esperadas = num_caracteristicas + 1;
        if (num_tokens != columnas_esperadas) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d tiene %d columnas, se esperaban %d.",
                    numero_linea, num_tokens, columnas_esperadas);
            mostrar_error(mensaje);
            errores_validacion++;
            continue;
        }
        
        // Validar y convertir cada valor de entrada
        int error_fila = 0;
        for (int j = 0; j < num_caracteristicas; j++) {
            float valor;
            if (!validar_y_convertir_numero_ds(tokens[j], &valor)) {
                char mensaje[256];
                sprintf(mensaje, "Error: Línea %d, columna %d: '%s' no es un número válido.",
                        numero_linea, j + 1, tokens[j]);
                mostrar_error(mensaje);
                errores_validacion++;
                error_fila = 1;
                break;
            }
            datos->entradas[muestra_actual][j] = valor;
        }
        
        if (error_fila) continue;
        
        // Validar y convertir la salida esperada (última columna)
        float salida;
        if (!validar_y_convertir_numero_ds(tokens[num_tokens - 1], &salida)) {
            char mensaje[256];
            sprintf(mensaje, "Error: Línea %d, última columna: '%s' no es un número válido.",
                    numero_linea, tokens[num_tokens - 1]);
            mostrar_error(mensaje);
            errores_validacion++;
            continue;
        }
        datos->salidas[muestra_actual] = salida;
        
        muestra_actual++;
    }
    
    fclose(f);
    
    // === VALIDACIÓN: Errores durante la lectura ===
    if (errores_validacion > 0) {
        char mensaje[256];
        sprintf(mensaje, "Error: Se encontraron %d errores al leer el archivo CSV.",
                errores_validacion);
        mostrar_error(mensaje);
        liberar_dataset(datos);
        return NULL;
    }
    
    // === VALIDACIÓN: Número de muestras leídas ===
    if (muestra_actual != num_muestras) {
        mostrar_error("Error: No se pudieron leer todas las muestras del CSV.");
        liberar_dataset(datos);
        return NULL;
    }
    
    // === DEPURACIÓN: Mostrar información del dataset ===
    printf("\n  [Depuración] Dataset cargado exitosamente:\n");
    printf("    - Archivo: %s\n", nombre_archivo);
    printf("    - Muestras: %d\n", datos->num_muestras);
    printf("    - Características: %d\n", datos->num_caracteristicas);
    
    return datos;
}

Dataset* cargar_dataset_desde_config(Configuracion *config) {
    if (config == NULL) {
        mostrar_error("Error: Configuración nula en cargar_dataset_desde_config.");
        return NULL;
    }
    return cargar_dataset(config->archivo_datos);
}

// ================================================================
// 3. FUNCIONES DE NORMALIZACIÓN
// ================================================================

void normalizar_dataset(Dataset *datos, float max_valor) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en normalizar_dataset.");
        return;
    }
    
    if (max_valor <= 0.0f) {
        mostrar_error("Error: max_valor debe ser mayor a 0 en normalizar_dataset.");
        return;
    }
    
    int contador = 0;
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            datos->entradas[i][j] = datos->entradas[i][j] / max_valor;
            // Asegurar que esté en el rango [0, 1]
            if (datos->entradas[i][j] < 0.0f) datos->entradas[i][j] = 0.0f;
            if (datos->entradas[i][j] > 1.0f) datos->entradas[i][j] = 1.0f;
            contador++;
        }
    }
    
    // ★ DEPURACIÓN: Mostrar resultado de normalización ★
    printf("\n  [Depuración] Normalización completada:\n");
    printf("    - Valores normalizados al rango [0, 1]\n");
    printf("    - Max_valor usado: %.2f\n", max_valor);
    printf("    - Total de valores normalizados: %d\n", contador);
}

void normalizar_dataset_auto(Dataset *datos) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en normalizar_dataset_auto.");
        return;
    }
    
    // Encontrar el valor máximo
    float max_valor = 0.0f;
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            if (datos->entradas[i][j] > max_valor) {
                max_valor = datos->entradas[i][j];
            }
        }
    }
    
    if (max_valor == 0.0f) {
        mostrar_error("Error: max_valor = 0 en normalizar_dataset_auto.");
        return;
    }
    
    // ★ DEPURACIÓN: Mostrar el máximo detectado ★
    printf("\n  [Depuración] Máximo detectado: %.2f\n", max_valor);
    
    normalizar_dataset(datos, max_valor);
}

void desnormalizar_dataset(Dataset *datos, float max_valor) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en desnormalizar_dataset.");
        return;
    }
    
    if (max_valor <= 0.0f) {
        mostrar_error("Error: max_valor debe ser mayor a 0 en desnormalizar_dataset.");
        return;
    }
    
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            datos->entradas[i][j] = datos->entradas[i][j] * max_valor;
        }
    }
    
    // ★ DEPURACIÓN: Mostrar resultado de desnormalización ★
    printf("\n  [Depuración] Desnormalización completada:\n");
    printf("    - Valores restaurados usando max_valor: %.2f\n", max_valor);
}

// ================================================================
// 4. FUNCIONES DE CONSULTA Y MANIPULACIÓN
// ================================================================

int obtener_num_muestras(Dataset *datos) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en obtener_num_muestras.");
        return -1;
    }
    return datos->num_muestras;
}

int obtener_num_caracteristicas(Dataset *datos) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en obtener_num_caracteristicas.");
        return -1;
    }
    return datos->num_caracteristicas;
}

float* obtener_muestra(Dataset *datos, int indice) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en obtener_muestra.");
        return NULL;
    }
    
    if (indice < 0 || indice >= datos->num_muestras) {
        char mensaje[256];
        sprintf(mensaje, "Error: Índice %d fuera de rango [0, %d] en obtener_muestra.",
                indice, datos->num_muestras - 1);
        mostrar_error(mensaje);
        return NULL;
    }
    
    return datos->entradas[indice];
}

float obtener_salida_esperada(Dataset *datos, int indice) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en obtener_salida_esperada.");
        return -1.0f;
    }
    
    if (indice < 0 || indice >= datos->num_muestras) {
        char mensaje[256];
        sprintf(mensaje, "Error: Índice %d fuera de rango en obtener_salida_esperada.",
                indice);
        mostrar_error(mensaje);
        return -1.0f;
    }
    
    return datos->salidas[indice];
}

DatasetInfo obtener_estadisticas_dataset(Dataset *datos) {
    DatasetInfo info = {0, 0, 0.0f, 0.0f, 0.0f, 0.0f};
    
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en obtener_estadisticas_dataset.");
        return info;
    }
    
    info.num_muestras = datos->num_muestras;
    info.num_caracteristicas = datos->num_caracteristicas;
    
    // Calcular mínimo, máximo y media
    float min_val = 1e10f;
    float max_val = -1e10f;
    float suma = 0.0f;
    int total_valores = 0;
    
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            float v = datos->entradas[i][j];
            if (v < min_val) min_val = v;
            if (v > max_val) max_val = v;
            suma += v;
            total_valores++;
        }
    }
    
    info.valor_minimo = min_val;
    info.valor_maximo = max_val;
    info.media = (total_valores > 0) ? suma / total_valores : 0.0f;
    
    // Calcular desviación estándar (aproximada)
    float suma_cuadrados = 0.0f;
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            float diff = datos->entradas[i][j] - info.media;
            suma_cuadrados += diff * diff;
        }
    }
    info.desviacion = (total_valores > 0) ? sqrtf(suma_cuadrados / total_valores) : 0.0f;
    
    return info;
}

// ================================================================
// 5. FUNCIONES DE VALIDACIÓN
// ================================================================

int validar_dataset(Dataset *datos) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en validar_dataset.");
        return 0;
    }
    
    // Validar que haya datos
    if (datos->num_muestras <= 0) {
        mostrar_error("Error: Dataset sin muestras.");
        return 0;
    }
    
    if (datos->num_caracteristicas <= 0) {
        mostrar_error("Error: Dataset sin características.");
        return 0;
    }
    
    // Validar que las matrices no sean NULL
    if (datos->entradas == NULL) {
        mostrar_error("Error: Matriz de entradas NULL.");
        return 0;
    }
    
    if (datos->salidas == NULL) {
        mostrar_error("Error: Vector de salidas NULL.");
        return 0;
    }
    
    // Validar cada fila
    for (int i = 0; i < datos->num_muestras; i++) {
        if (datos->entradas[i] == NULL) {
            char mensaje[256];
            sprintf(mensaje, "Error: Fila %d de entradas NULL.", i);
            mostrar_error(mensaje);
            return 0;
        }
        
        // Validar que las salidas estén en rango [0, 1]
        if (datos->salidas[i] < 0.0f || datos->salidas[i] > 1.0f) {
            char mensaje[256];
            sprintf(mensaje, "Error: Salida %d fuera de rango [0, 1]: %.2f",
                    i, datos->salidas[i]);
            mostrar_error(mensaje);
            return 0;
        }
    }
    
    // Validar que no haya NaN o infinitos
    for (int i = 0; i < datos->num_muestras; i++) {
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            float v = datos->entradas[i][j];
            if (isnan(v) || isinf(v)) {
                char mensaje[256];
                sprintf(mensaje, "Error: Valor inválido en [%d][%d]: %f", i, j, v);
                mostrar_error(mensaje);
                return 0;
            }
        }
    }
    
    return 1;
}

// ================================================================
// 6. FUNCIONES DE DEPURACIÓN Y VISUALIZACIÓN
// ================================================================

void imprimir_dataset(Dataset *datos, int max_muestras) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en imprimir_dataset.");
        return;
    }
    
    printf("\n=== CONTENIDO DEL DATASET ===\n");
    printf("  Muestras: %d | Características: %d\n\n", 
           datos->num_muestras, datos->num_caracteristicas);
    
    // Determinar cuántas muestras mostrar
    int mostrar = (max_muestras == -1) ? datos->num_muestras : max_muestras;
    if (mostrar > datos->num_muestras) mostrar = datos->num_muestras;
    
    // Imprimir encabezados
    printf("  Muestra | ");
    for (int j = 0; j < datos->num_caracteristicas; j++) {
        printf("X%-2d ", j + 1);
        if (j < datos->num_caracteristicas - 1) printf("| ");
    }
    printf("| Salida\n");
    
    printf("  --------+");
    for (int j = 0; j < datos->num_caracteristicas; j++) {
        printf("-----");
        if (j < datos->num_caracteristicas - 1) printf("+");
    }
    printf("+-------\n");
    
    // Imprimir datos
    for (int i = 0; i < mostrar; i++) {
        printf("  %3d     | ", i + 1);
        for (int j = 0; j < datos->num_caracteristicas; j++) {
            printf("%.2f ", datos->entradas[i][j]);
            if (j < datos->num_caracteristicas - 1) printf("| ");
        }
        printf("| %.2f\n", datos->salidas[i]);
    }
    
    if (mostrar < datos->num_muestras) {
        printf("  ... (%d muestras adicionales)\n", 
               datos->num_muestras - mostrar);
    }
    
    printf("==============================\n\n");
}

void imprimir_estadisticas_dataset(Dataset *datos) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en imprimir_estadisticas_dataset.");
        return;
    }
    
    DatasetInfo info = obtener_estadisticas_dataset(datos);
    
    printf("\n=== ESTADÍSTICAS DEL DATASET ===\n");
    printf("  Muestras:         %d\n", info.num_muestras);
    printf("  Características:  %d\n", info.num_caracteristicas);
    printf("  Valor mínimo:     %.6f\n", info.valor_minimo);
    printf("  Valor máximo:     %.6f\n", info.valor_maximo);
    printf("  Media:            %.6f\n", info.media);
    printf("  Desviación:       %.6f\n", info.desviacion);
    printf("================================\n\n");
}

void imprimir_matriz_como_imagen(Dataset *datos, int indice, int filas, int columnas) {
    if (datos == NULL) {
        mostrar_error("Error: Dataset nulo en imprimir_matriz_como_imagen.");
        return;
    }
    
    if (indice < 0 || indice >= datos->num_muestras) {
        char mensaje[256];
        sprintf(mensaje, "Error: Índice %d fuera de rango.", indice);
        mostrar_error(mensaje);
        return;
    }
    
    int total_pixeles = filas * columnas;
    if (total_pixeles != datos->num_caracteristicas) {
        char mensaje[256];
        sprintf(mensaje, "Error: %d×%d = %d píxeles, pero hay %d características.",
                filas, columnas, total_pixeles, datos->num_caracteristicas);
        mostrar_error(mensaje);
        return;
    }
    
    printf("\n=== IMAGEN (Muestra %d) ===\n", indice + 1);
    printf("  ");
    for (int j = 0; j < columnas; j++) {
        printf("---");
    }
    printf("\n");
    
    for (int i = 0; i < filas; i++) {
        printf("  ");
        for (int j = 0; j < columnas; j++) {
            int idx = i * columnas + j;
            float valor = datos->entradas[indice][idx];
            char simbolo;
            
            // Mapear valor 0-1 a caracteres ASCII
            if (valor < 0.1f) simbolo = ' ';
            else if (valor < 0.25f) simbolo = '.';
            else if (valor < 0.45f) simbolo = '*';
            else if (valor < 0.65f) simbolo = 'o';
            else if (valor < 0.85f) simbolo = '#';
            else simbolo = '@';
            
            printf(" %c ", simbolo);
        }
        printf("\n");
    }
    
    printf("  ");
    for (int j = 0; j < columnas; j++) {
        printf("---");
    }
    printf("\n");
    printf("  Salida esperada: %.2f\n", datos->salidas[indice]);
    printf("========================\n\n");
}

// ================================================================
// 7. FUNCIONES DE MEMORIA
// ================================================================

void liberar_dataset(Dataset *datos) {
    if (datos != NULL) {
        // Liberar cada fila de entradas
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
        
        // Liberar vector de salidas
        if (datos->salidas != NULL) {
            free(datos->salidas);
            datos->salidas = NULL;
        }
        
        // Liberar la estructura principal
        free(datos);
        datos = NULL;
        
        printf("\n  [Depuración] Dataset liberado correctamente.\n");
    }
}

void liberar_dataset_seguro(Dataset **datos) {
    if (datos != NULL && *datos != NULL) {
        liberar_dataset(*datos);
        *datos = NULL;
    }
}

// ================================================================
// FIN DEL ARCHIVO dataset.c
// ================================================================