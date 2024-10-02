#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void manejador_signal(int signo) {
    printf("Recibí la señal: %d\n", signo);
}

int main() {
    signal(2, manejador_signal);
    signal(16, manejador_signal);
    signal(17, manejador_signal);
    signal(19, manejador_signal);

    printf("Esperando señales...\n");

    while (1) {
        pause();
    }

    return 0;
}
