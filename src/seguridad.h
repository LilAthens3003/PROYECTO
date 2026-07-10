#ifndef SEGURIDAD_H
#define SEGURIDAD_H

/* Declara las funciones globales para interceptar eventos del teclado a nivel del sistema operativo.
 * Se utiliza para evitar que el usuario ingrese caracteres inválidos y rompa la matemática del modelo.
 * Se aísla aquí para no contaminar el resto de los módulos con la librería <windows.h>. */
void ActivarSeguridadTeclado(void);
void DesactivarSeguridadTeclado(void);

#endif