#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 128

struct msgbuf {
    long mtype;
    char mtext[MAXSIZE];
};

void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    int msqid;
    key_t key = 1234;
    struct msgbuf sbuf;
    size_t buflen;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        die("msgget");
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

        sbuf.mtype = 1;
        switch (opcion) {
            case 1:
                strcpy(sbuf.mtext, "2");
                break;
            case 2:
                strcpy(sbuf.mtext, "16");
                break;
            case 3:
                strcpy(sbuf.mtext, "17");
                break;
            case 4:
                strcpy(sbuf.mtext, "19");
                break;
            default:
                printf("Opción inválida\n");
                continue;
        }

        buflen = strlen(sbuf.mtext) + 1;
        if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) {
            die("msgsnd");
        }

        printf("Mensaje enviado: %s\n", sbuf.mtext);
        if (strcmp(sbuf.mtext, "19") == 0) {
            break;
        }
    }

    return 0;
}
