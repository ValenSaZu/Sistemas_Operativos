#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define MAXSIZE 128

struct msgbuf {
    long mtype;
    char mtext[MAXSIZE];
};

void die(char *s) {
    perror(s);
    exit(1);
}

void enviar_signal(int signal) {
    pid_t pid = getppid();
    kill(pid, signal);
}

void *thread_func(void *arg) {
    int msqid = *((int *)arg);
    struct msgbuf rcvbuffer;

    while (1) {
        if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0) {
            die("msgrcv");
        }

        int signal = atoi(rcvbuffer.mtext);
        printf("Thread recibió señal: %d\n", signal);
        enviar_signal(signal);

        if (signal == 19) {
            break;
        }
    }

    return NULL;
}

int main() {
    key_t key = 1234;
    int msqid;

    if ((msqid = msgget(key, 0666)) < 0) {
        die("msgget");
    }

    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread_func, &msqid);
    pthread_create(&tid2, NULL, thread_func, &msqid);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
