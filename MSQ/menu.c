#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_MAXIMO 128

struct mensaje {
    long tipo;
    char texto[TAM_MAXIMO];
};

void error(char *mensaje) {
    perror(mensaje);
    exit(1);
}

int main() {
    int id_cola_mensajes;
    key_t clave = 1234;
    struct mensaje mensaje_enviar;
    size_t longitud_mensaje;

    if ((id_cola_mensajes = msgget(clave, IPC_CREAT | 0666)) < 0) {
        error("msgget");
    }

    while (1) {
        int opcion;
        printf("Menu:\n");
        printf("1. Enviar señal 2\n");
        printf("2. Enviar señal 16\n");
        printf("3. Enviar señal 17\n");
        printf("4. Enviar señal de parada (19)\n");
        printf("Elija una opción: ");
        scanf("%d", &opcion);
        getchar();

        mensaje_enviar.tipo = 1;
        switch (opcion) {
            case 1:
                strcpy(mensaje_enviar.texto, "2");
                break;
            case 2:
                strcpy(mensaje_enviar.texto, "16");
                break;
            case 3:
                strcpy(mensaje_enviar.texto, "17");
                break;
            case 4:
                strcpy(mensaje_enviar.texto, "19");
                break;
            default:
                printf("Opción inválida\n");
                continue;
        }

        longitud_mensaje = strlen(mensaje_enviar.texto) + 1;
        if (msgsnd(id_cola_mensajes, &mensaje_enviar, longitud_mensaje, IPC_NOWAIT) < 0) {
            error("msgsnd");
        }

        printf("Mensaje enviado: %s\n", mensaje_enviar.texto);
        if (strcmp(mensaje_enviar.texto, "19") == 0) {
            break;
        }
    }

    return 0;
}