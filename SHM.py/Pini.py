import mmap
import os
import posix_ipc

SHM_NAME = "/shmfile"

shm = posix_ipc.SharedMemory(SHM_NAME, posix_ipc.O_CREX, size=1024)
memory = mmap.mmap(shm.fd, shm.size)
os.close(shm.fd)

memory.write(b"003INIT")
print("Memoria compartida inicializada: 003INIT")

memory.close()
