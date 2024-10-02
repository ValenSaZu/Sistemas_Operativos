#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void manejador_senal(int numero_senal) {
    printf("Recibí la señal: %d\n", numero_senal);
}

int main() {
    signal(2, manejador_senal);
    signal(16, manejador_senal);
    signal(17, manejador_senal);
    signal(19, manejador_senal);

    printf("Esperando señales...\n");

    while (1) {
        pause();
    }

    return 0;
}