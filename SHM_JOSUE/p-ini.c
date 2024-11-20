#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 1024

int main() {
    int shmid, semid;
    char *shmaddr;

    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error al crear memoria compartida");
        return 1;
    }

    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("Error al adjuntar memoria compartida");
        return 1;
    }

    strcpy(shmaddr, "003INIT");

    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("Error al crear semáforo");
        return 1;
    }

    if (semctl(semid, 0, SETVAL, 1) < 0) {
        perror("Error al inicializar semáforo");
        return 1;
    }

    printf("Memoria compartida inicializada correctamente: %s\n", shmaddr);

    return 0;
}