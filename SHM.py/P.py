import mmap
import os
import sys
import posix_ipc

if len(sys.argv) != 3:
    print(f"Uso: {sys.argv[0]} <string> <numero>")
    sys.exit(1)

SHM_NAME = "/shmfile"
input_string = sys.argv[1]
num = int(sys.argv[2])

shm = posix_ipc.SharedMemory(SHM_NAME)
memory = mmap.mmap(shm.fd, shm.size)
os.close(shm.fd)

sem = posix_ipc.Semaphore("/mysem", posix_ipc.O_CREAT, initial_value=1)

sem.acquire()

memory.seek(2)
if memory.read(1) == b'3' and num == 3:
    memory.seek(3)
    memory.write(input_string.encode())
    memory.seek(0)
    print("Memoria modificada:", memory.read().decode())
else:
    print("El número no coincide con 3.")

sem.release()

memory.close()
