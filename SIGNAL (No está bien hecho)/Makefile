# Nombre de los ejecutables
SENDER = ./signal_sender.exe
RECEIVER = ./signal_receiver.exe

# Archivos fuente
SENDER_SRC = signal_sender.c
RECEIVER_SRC = signal_receiver.c

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -O2

# Regla por defecto
all: $(SENDER) $(RECEIVER)

# Regla para compilar el programa signal_sender
$(SENDER): $(SENDER_SRC)
	$(CC) $(CFLAGS) -o $(SENDER) $(SENDER_SRC)

# Regla para compilar el programa signal_receiver
$(RECEIVER): $(RECEIVER_SRC)
	$(CC) $(CFLAGS) -o $(RECEIVER) $(RECEIVER_SRC)

# Regla para limpiar los archivos generados
clean:
	del /Q $(SENDER) $(RECEIVER)