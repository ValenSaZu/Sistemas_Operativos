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

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Error al crear las tuberías");
        return 1;
    }

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

        write(pipe1[1], EOM, strlen(EOM) + 1);
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

        int eom_found = 0;
        while (!eom_found) {
            int bytes_read = read(pipe1[0], buffer, MSG_SIZE - 1);
            if (bytes_read <= 0) {
                break;
            }
            buffer[bytes_read] = '\0';

            char *eom_pos = strstr(buffer, EOM);
            if (eom_pos != NULL) {
                *eom_pos = '\0';
                eom_found = 1;
            }

            char *line = strtok(buffer, "\n");
            while (line != NULL) {
                line_count++;
                fprintf(file_numerado, "%d: %s\n", line_count, line);
                line = strtok(NULL, "\n");
            }
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
