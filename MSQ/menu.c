#include <sys/types.h> //key_t: 
#include <sys/ipc.h> // mssget: , IPC_CREATE:
#include <sys/msg.h> // 
#include <stdio.h> // printf: escribir, scanf: leer, perror: depura problemas del sistema, getchar: limpia el buffer de entrada
#include <stdlib.h> //  exit: termina ejecución y devuelve un estado
#include <string.h> //strcpy: Copia string de un origen a un destino (destino, contenido)
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
    key_t clave = 1234; // clave para la cola de mensajes
    struct mensaje mensaje_enviar;
    size_t longitud_mensaje;

    if ((id_cola_mensajes = msgget(clave, IPC_CREAT | 0666)) < 0) { //lectura y escritura: 066, devuelve un identificador de la cola de mensajes
        error("msgget");
    }

    while (1) {
        int opcion, tipo_mensaje;
        printf("Menu:\n");
        printf("1. Enviar señal 2\n");
        printf("2. Enviar señal 16\n");
        printf("3. Enviar señal 17\n");
        printf("4. Enviar señal de parada (19)\n");
        printf("Elija una opción: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                tipo_mensaje = 1;
                strcpy(mensaje_enviar.texto, "2");
                break;
            case 2:
                tipo_mensaje = 1;
                strcpy(mensaje_enviar.texto, "16");
                break;
            case 3:
                tipo_mensaje = 2;
                strcpy(mensaje_enviar.texto, "17");
                break;
            case 4:
                tipo_mensaje = 2;
                strcpy(mensaje_enviar.texto, "19");
                break;
            default:
                printf("Opción inválida\n");
                continue;
        }

        mensaje_enviar.tipo = tipo_mensaje;
        longitud_mensaje = strlen(mensaje_enviar.texto) + 1;
        if (msgsnd(id_cola_mensajes, &mensaje_enviar, longitud_mensaje, IPC_NOWAIT) < 0) { //envia mensaje a la cola
            error("msgsnd");
        }

        printf("Mensaje enviado al tipo %d: %s\n", tipo_mensaje, mensaje_enviar.texto);
        if (strcmp(mensaje_enviar.texto, "19") == 0) { //commpara strings
            break;
        }
    }

    return 0;
}