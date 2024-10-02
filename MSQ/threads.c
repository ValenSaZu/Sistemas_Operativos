#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define TAM_MAXIMO 128

struct mensaje {
    long tipo;
    char texto[TAM_MAXIMO];
};

void error(char *mensaje) {
    perror(mensaje);
    exit(1);
}

void enviar_signal(int signal) {
    pid_t pid = getppid();
    kill(pid, signal);
}

void *funcion_hilo(void *arg) {
    int id_cola_mensajes = *((int *)arg);
    struct mensaje buffer_recibir;

    while (1) {
        if (msgrcv(id_cola_mensajes, &buffer_recibir, TAM_MAXIMO, 1, 0) < 0) {
            error("msgrcv");
        }

        int signal = atoi(buffer_recibir.texto);
        printf("Hilo recibió señal: %d\n", signal);
        enviar_signal(signal);

        if (signal == 19) {
            break;
        }
    }

    return NULL;
}

int main() {
    key_t clave = 1234;
    int id_cola_mensajes;

    if ((id_cola_mensajes = msgget(clave, 0666)) < 0) {
        error("msgget");
    }

    pthread_t hilo1, hilo2;

    pthread_create(&hilo1, NULL, funcion_hilo, &id_cola_mensajes);
    pthread_create(&hilo2, NULL, funcion_hilo, &id_cola_mensajes);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}