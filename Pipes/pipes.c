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

    // Crear las tuberías
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Error al crear las tuberías");
        return 1;
    }

    // Crear el proceso hijo
    pid = fork();

    if (pid == 0) { // Proceso hijo
        // Cerrar los extremos no utilizados
        close(pipe1[0]); // Cierra la lectura de pipe1
        close(pipe2[1]); // Cierra la escritura de pipe2

        // Abrir el archivo "archivo.txt" para leer
        FILE *file = fopen("archivo.txt", "r");
        if (file == NULL) {
            perror("Error al abrir archivo.txt");
            return 1;
        }

        // Leer líneas del archivo y escribirlas en la tubería pipe1
        while (fgets(buffer, MSG_SIZE, file)) {
            write(pipe1[1], buffer, strlen(buffer));
        }
        fclose(file);

        // Enviar señal de finalización "EOM"
        write(pipe1[1], EOM, strlen(EOM) + 1);
        close(pipe1[1]); // Cerrar la escritura de pipe1

        // Leer el número de líneas desde pipe2
        read(pipe2[0], buffer, MSG_SIZE);
        FILE *file_num = fopen("numero_lineas.txt", "w");
        fprintf(file_num, "%s", buffer);
        fclose(file_num);
        close(pipe2[0]); // Cerrar la lectura de pipe2
    } 
    else { // Proceso padre
        // Cerrar los extremos no utilizados
        close(pipe1[1]); // Cierra la escritura de pipe1
        close(pipe2[0]); // Cierra la lectura de pipe2

        // Abrir el archivo "archivo_con_numeros.txt" para escribir
        FILE *file_numerado = fopen("archivo_con_numeros.txt", "w");

        // Leer desde la tubería pipe1 y enumerar las líneas
        while (1) {
            int bytes_read = read(pipe1[0], buffer, MSG_SIZE - 1);
            if (bytes_read <= 0) {
                break; // Si no se leen bytes, romper el bucle
            }
            buffer[bytes_read] = '\0';

            // Verificar si se ha recibido el marcador de final (EOM)
            if (strcmp(buffer, EOM) == 0) {
                break;
            }

            // Contar líneas en base a los saltos de línea
            char *line = strtok(buffer, "\n");
            while (line != NULL) {
                line_count++;
                fprintf(file_numerado, "%d: %s\n", line_count, line);
                line = strtok(NULL, "\n");
            }
        }
        fclose(file_numerado);
        close(pipe1[0]); // Cerrar la lectura de pipe1

        // Enviar el número de líneas al hijo a través de pipe2
        sprintf(buffer, "%d", line_count);
        write(pipe2[1], buffer, strlen(buffer) + 1);
        close(pipe2[1]); // Cerrar la escritura de pipe2

        // Esperar a que el proceso hijo termine
        wait(NULL);
    }

    return 0;
}
