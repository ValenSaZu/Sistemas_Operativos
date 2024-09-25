#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int input_fd, output_fd;

    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        return 1;
    }

    if (pid == 0) {
        input_fd = open("input_task.txt", O_RDONLY);
        if (input_fd < 0) {
            perror("Error al abrir el archivo de entrada");
            exit(1);
        }

        output_fd = open("output_task.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            perror("Error al abrir el archivo de salida");
            close(input_fd);
            exit(1);
        }

        dup2(input_fd, STDIN_FILENO);
        close(input_fd);

        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        char *args[] = {"./programa1", NULL};
        execvp(args[0], args);

        perror("Error al ejecutar el programa 1");
        exit(1);
    } else {
        wait(NULL);
        printf("El proceso hijo ha terminado.\n");
    }

    return 0;
}

