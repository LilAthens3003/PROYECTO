#define WIN32_WINNT 0x0501
#include <windows.h>
#include "seguridad.h"

// Variable estática para almacenar el manejador del hook y limitar su alcance a este módulo.
static HHOOK ghHook = NULL;

/* Evalúa los Virtual Key Codes (VK) para permitir únicamente la entrada de caracteres
 * numéricos, controles de borrado y separadores decimales necesarios para la interfaz. */
BOOL IsAllowedVk(DWORD vk) {
    if (vk == VK_BACK) return TRUE;
    if (vk == VK_DELETE) return TRUE;
    if (vk >= 0x30 && vk <= 0x39) return TRUE;
    if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) return TRUE;
    if (vk == VK_OEM_PERIOD) return TRUE;
    if (vk == VK_DECIMAL) return TRUE;

    return FALSE;
}

/* Procedimiento de callback que intercepta los eventos de teclado de bajo nivel (WH_KEYBOARD_LL)
 * antes de que el sistema operativo o la aplicación los procesen de manera estándar. */
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < HC_ACTION) {
        return CallNextHookEx(ghHook, nCode, wParam, lParam);
    }

    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
    DWORD vk = p->vkCode;

    // Se evalúan las pulsaciones y se descartan combinaciones de teclas con modificadores.
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        BOOL blockThis = FALSE;

        BOOL shiftDown = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        BOOL ctrlDown  = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
        BOOL altDown   = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
        BOOL winDown   = (GetAsyncKeyState(VK_LWIN) & 0x8000) != 0 || (GetAsyncKeyState(VK_RWIN) & 0x8000) != 0;

        if (ctrlDown || altDown || winDown) {
            blockThis = TRUE;
        }

        if (!IsAllowedVk(vk)) {
            blockThis = TRUE;
        }

        if (shiftDown && vk >= 0x30 && vk <= 0x39) {
            blockThis = TRUE;
        }

        if (shiftDown && vk == VK_OEM_PERIOD) {
            blockThis = TRUE;
        }

        if (shiftDown && vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) {
            blockThis = TRUE;
        }

        if (shiftDown && vk == VK_DECIMAL) {
            blockThis = TRUE;
        }

        // Si la tecla se marca como bloqueada, se retorna 1 para suprimir su procesamiento.
        if (blockThis) {
            return 1;
        }
    }

    return CallNextHookEx(ghHook, nCode, wParam, lParam);
}

/* Instala el hook en la cadena del sistema operativo para habilitar el filtrado global. */
void ActivarSeguridadTeclado(void) {
    if (ghHook == NULL) {
        ghHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    }
}

/* Remueve el hook activo y restaura el control de entrada estándar del sistema operativo. */
void DesactivarSeguridadTeclado(void) {
    if (ghHook != NULL) {
        UnhookWindowsHookEx(ghHook);
        ghHook = NULL;
    }
}