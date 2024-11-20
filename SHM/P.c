#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <string> <numero>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[2]);
    int shm_id;
    char *shm_ptr;
    key_t key = ftok("shmfile", 65);

    shm_id = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    shm_ptr = (char *)shmat(shm_id, NULL, 0);

    sem_t *sem = sem_open("/mysem", O_CREAT, 0644, 1);
    
    sem_wait(sem);

    if (shm_ptr[2] == '3' && num == 3) {
        snprintf(shm_ptr + 3, SHM_SIZE - 2, "%s", argv[1]);
        printf("Memoria modificada: %s\n", shm_ptr);
    } else {
        printf("El número no coincide con 3.\n");
    }

    sem_post(sem);

    shmdt(shm_ptr);

    sem_close(sem);
    sem_unlink("/mysem");

    return 0;
}
