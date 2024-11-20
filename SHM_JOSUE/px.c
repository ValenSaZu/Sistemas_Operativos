#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 1024

struct sembuf sem_op;

int main(int argc, char *argv[]) {
    int shmid, semid;
    char *shmaddr;
    int num;
    char *input_string;

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Uso: %s <número> [string]\n", argv[0]);
        return 1;
    }

    num = atoi(argv[1]);
    if (num < 0 || num > 999) {
        fprintf(stderr, "El número debe estar entre 0 y 999.\n");
        return 1;
    }

    if (argc == 3) {
        input_string = argv[2];
    } else {
        input_string = "ucsp";
    }

    char new_content[SHM_SIZE];
    snprintf(new_content, sizeof(new_content), "%03d%s", num, input_string);

    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("Error al obtener memoria compartida");
        return 1;
    }

    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("Error al adjuntar memoria compartida");
        return 1;
    }

    semid = semget(SEM_KEY, 1, 0666);
    if (semid < 0) {
        perror("Error al obtener semáforo");
        return 1;
    }

    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    if (semop(semid, &sem_op, 1) < 0) {
        perror("Error al bajar el semáforo");
        return 1;
    }

    if (num == 3) {
        strncpy(shmaddr, new_content, SHM_SIZE - 1);
        shmaddr[SHM_SIZE - 1] = '\0';
        printf("Memoria compartida actualizada a: %s\n", new_content);
    } else {
        printf("El número no coincide con 3. No se realizó la modificación.\n");
    }

    sem_op.sem_op = 1;
    if (semop(semid, &sem_op, 1) < 0) {
        perror("Error al subir el semáforo");
        return 1;
    }

    if (shmdt(shmaddr) < 0) {
        perror("Error al desadjuntar memoria compartida");
        return 1;
    }

    return 0;
}