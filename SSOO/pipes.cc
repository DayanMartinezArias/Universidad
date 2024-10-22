#include <iostream>
#include <unistd.h>

int main() {
    int fd[2];  // fd[0] para lectura, fd[1] para escritura
    pipe(fd);

    if (fork() == 0) {  // Proceso hijo
        close(fd[0]);   // Cerrar lado de lectura
        char message[] = "Hola desde el hijo";
        write(fd[1], message, sizeof(message));
        close(fd[1]);
    } else {  // Proceso padre
        close(fd[1]);   // Cerrar lado de escritura
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        std::cout << "Mensaje recibido: " << buffer << std::endl;
        close(fd[0]);
    }

    return 0;
}
