#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SHMSZ 27

int main() {
    int shmid;
    key_t key;
    char* shm;

    key = 5678;
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("Error al crear memoria compartida (shmget)");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("Error al adjuntar memoria compartida (shmat)");
        exit(1);
    }

    sem_t* sem_w = sem_open("/sem_w", O_CREAT, 0666, 1);
    sem_t* sem_r = sem_open("/sem_r", O_CREAT, 0666, 0);

    if (sem_w == SEM_FAILED || sem_r == SEM_FAILED) {
        perror("Error al crear los semáforos");
        exit(1);
    }

    sem_wait(sem_w);

    strcpy(shm, "0003INIT");
    printf("Memoria compartida inicializada con '0003INIT'.\n");

    sem_post(sem_r);

    sem_close(sem_w);
    sem_close(sem_r);

    return 0;
}