#define _WIN32_WINNT 0x0501
#include <windows.h>
#include "seguridad.h"

// Variable global (ahora solo vive en este archivo)
HHOOK g_hHook = NULL;

BOOL IsAllowedVk(DWORD vk) {
    // Números 0-9
    if (vk >= 0x30 && vk <= 0x39) return TRUE;
    // Letras A-Z
    if (vk >= 0x41 && vk <= 0x5A) return TRUE;
    
    return FALSE;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vk = p->vkCode;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            BOOL ctrlDown  = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) || (GetAsyncKeyState(VK_RCONTROL) & 0x8000);
            BOOL altDown   = (GetAsyncKeyState(VK_LMENU) & 0x8000) || (GetAsyncKeyState(VK_RMENU) & 0x8000);
            BOOL winDown   = (GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000);

            BOOL blockThis = FALSE;

            if (vk == VK_LWIN || vk == VK_RWIN) blockThis = TRUE;
            if (vk == VK_ESCAPE || vk == VK_DELETE || vk == VK_INSERT) blockThis = TRUE;
            if (vk >= VK_F1 && vk <= VK_F12) blockThis = TRUE;
            
            if (vk == VK_SHIFT    || vk == VK_LSHIFT    || vk == VK_RSHIFT ||
                vk == VK_CONTROL  || vk == VK_LCONTROL  || vk == VK_RCONTROL ||
                vk == VK_MENU     || vk == VK_LMENU     || vk == VK_RMENU) {
                blockThis = TRUE;
            }

            if (ctrlDown || altDown || winDown) blockThis = TRUE;
            if (!IsAllowedVk(vk)) blockThis = TRUE;

            if (blockThis) return 1; // Windows ignora la tecla por completo
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// ESTO ERA LO QUE TE FALTABA: La función que instala el Hook
void ActivarSeguridadTeclado(void) {
    if (g_hHook == NULL) {
        g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    }
}

// Y la función que lo quita
void DesactivarSeguridadTeclado(void) {
    if (g_hHook != NULL) {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
}