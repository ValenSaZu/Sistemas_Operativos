# Sistemas_Operativos

sudo apt-get install git  
sudo apt install make  
sudo apt install build-essential

- Primera tarea Makefiles  
make -f Makefile-dynamic  
./p_dynamic.exe


- Tercera tarea pipes  
gcc pipes.c -o pipes  
./pipes


- Tarea dup2  
gcc programa1.c -o programa1  
gcc programa2.c -o programa2  
./programa2  


- TAREA MSQ  
gcc menu.c -o menu  
gcc threads.c -o threads -lpthread  
gcc receiver.c -o receiver  
./receiver  
./threads  
./menu

- TAREA SHM
gcc -o Pini Pini.c -pthread  
gcc -o P P.c -pthread  
./Pini  
./P <nombre> <numero>  
Limpieza de semaphoras después de ejecutar:  
sem_unlink("/sem_w");  
sem_unlink("/sem_r");  

