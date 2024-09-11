#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MSG_SIZE 100
#define EOM "EOM"

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;
    char buffer[MSG_SIZE];
    int line_count = 0;

    pipe(pipe1);
    pipe(pipe2);

    pid = fork();

    if (pid == 0) {
        close(pipe1[0]);
        close(pipe2[1]);

        FILE *file = fopen("archivo.txt", "r");
        if (file == NULL) {
            perror("Error al abrir archivo.txt");
            return 1;
        }

        while (fgets(buffer, MSG_SIZE, file)) {
            write(pipe1[1], buffer, strlen(buffer));
        }
        fclose(file);

        write(pipe1[1], EOM, strlen(EOM));
        close(pipe1[1]);

        read(pipe2[0], buffer, MSG_SIZE);
        FILE *file_num = fopen("numero_lineas.txt", "w");
        fprintf(file_num, "%s", buffer);
        fclose(file_num);
        close(pipe2[0]);
    } 
    else {
        close(pipe1[1]);
        close(pipe2[0]);

        FILE *file_numerado = fopen("archivo_con_numeros.txt", "w");

        while (1) {
            int bytes_read = read(pipe1[0], buffer, MSG_SIZE);
            buffer[bytes_read] = '\0';
            if (strcmp(buffer, EOM) == 0) {
                break;
            }
            line_count++;
            fprintf(file_numerado, "%d: %s", line_count, buffer);
        }
        fclose(file_numerado);
        close(pipe1[0]);

        sprintf(buffer, "%d", line_count);
        write(pipe2[1], buffer, strlen(buffer) + 1);
        close(pipe2[1]);

        wait(NULL);
    }

    return 0;
}
