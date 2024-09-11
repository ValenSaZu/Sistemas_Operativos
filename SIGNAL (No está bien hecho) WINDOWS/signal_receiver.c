#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

BOOL WINAPI consoleHandler(DWORD signal) {
    switch (signal) {
        case CTRL_C_EVENT:
            printf("Señal CTRL+C (continue) recibida\n");
            return TRUE;
        case CTRL_BREAK_EVENT:
            printf("Señal CTRL+BREAK (stop) recibida\n");
            return TRUE;
        default:
            return FALSE;
    }
}

int main() {
    // Configurar el manejador de eventos de consola
    if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
        fprintf(stderr, "Error configurando el manejador de eventos de consola\n");
        return 1;
    }

    printf("Esperando señales... Presiona CTRL+C o CTRL+BREAK para enviar señales...\n");

    // Mantener el proceso en espera para recibir señales
    while (TRUE) {
        Sleep(1000); // Dormir por 1 segundo
    }

    return 0;
}
