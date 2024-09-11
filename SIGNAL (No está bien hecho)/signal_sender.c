#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <signal> <Process ID>\n", argv[0]);
        fprintf(stderr, "Signal 1: stop (CTRL_BREAK_EVENT)\n");
        fprintf(stderr, "Signal 2: continue (CTRL_C_EVENT)\n");
        return 1;
    }

    DWORD signal;
    if (strcmp(argv[1], "1") == 0) {
        signal = CTRL_BREAK_EVENT;
    } else if (strcmp(argv[1], "2") == 0) {
        signal = CTRL_C_EVENT;
    } else {
        fprintf(stderr, "Señal no válida. Use '1' para stop o '2' para continue.\n");
        return 1;
    }

    DWORD process_id = atoi(argv[2]);

    if (!GenerateConsoleCtrlEvent(signal, process_id)) {
        fprintf(stderr, "Error enviando la señal: %lu\n", GetLastError());
        return 1;
    }

    printf("Señal '%s' enviada al proceso %lu\n", argv[1], process_id);
    return 0;
}