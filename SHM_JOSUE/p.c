#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 1024

struct sembuf sem_op;

int main() {
    int shmid, semid;
    char *shmaddr;

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

    while (1) {
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);

        printf("Contenido de la memoria compartida: %s\n", shmaddr);

        sem_op.sem_op = 1;
        semop(semid, &sem_op, 1);

        sleep(1);
    }

    return 0;
}