#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    int shm_id;
    char *shm_ptr;
    key_t key = ftok("shmfile", 65);

    shm_id = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    shm_ptr = (char *)shmat(shm_id, NULL, 0);

    sprintf(shm_ptr, "003INIT");
    printf("Memoria compartida inicializada: %s\n", shm_ptr);

    shmdt(shm_ptr);

    return 0;
}
