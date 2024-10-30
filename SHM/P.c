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

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nombre> <numero>\n", argv[0]);
        exit(1);
    }

    int shmid;
    key_t key;
    char* shm;
    int v = atoi(argv[2]);

    key = 5678;
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("Error al crear memoria compartida (shmget)");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("Error al adjuntar memoria compartida (shmat)");
        exit(1);
    }

    sem_t* sem_w = sem_open("/sem_w", 0);
    sem_t* sem_r = sem_open("/sem_r", 0);

    if (sem_w == SEM_FAILED || sem_r == SEM_FAILED) {
        perror("Error al abrir los semáforos");
        exit(1);
    }

    sem_wait(sem_r);

    char str_val[4];
    strncpy(str_val, shm, 3);
    str_val[3] = '\0';
    int val_en_memoria = atoi(str_val);

    if (val_en_memoria == v) {
        sem_wait(sem_w);

        sprintf(shm, "%03d%s", v, argv[1]);
        printf("Actualizado en memoria compartida: %03d%s\n", v, argv[1]);

        sem_post(sem_w);
    }
    else {
        printf("Valor en memoria (%03d) no coincide con el valor ingresado (%03d).\n", val_en_memoria, v);
    }

    sem_post(sem_r);

    sem_close(sem_w);
    sem_close(sem_r);

    return 0;
}
