import mmap
import os
import posix_ipc

SHM_NAME = "/shmfile"

shm = posix_ipc.SharedMemory(SHM_NAME)
memory = mmap.mmap(shm.fd, shm.size)
os.close(shm.fd)

memory.seek(0)
print("Contenido de la memoria compartida:", memory.read().decode())

memory.close()
