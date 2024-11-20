#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    int shm_id;
    char *shm_ptr;
    key_t key = ftok("shmfile", 65);

    shm_id = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    shm_ptr = (char *)shmat(shm_id, NULL, 0);

    printf("Contenido de la memoria compartida: %s\n", shm_ptr);

    shmdt(shm_ptr);

    return 0;
}
