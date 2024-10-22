#include <iostream>
#include <fcntl.h>    // Para open()
#include <unistd.h>   // Para write()
#include <cstring>

int main() {
    const char* path = "./mi_fifo";
    int fd = open(path, O_WRONLY);  // Abrir el FIFO en modo escritura

    const char* mensaje = "Mensaje a través de FIFO";
    write(fd, mensaje, strlen(mensaje) + 1);  // Escribir mensaje en el FIFO
    close(fd);  // Cerrar el descriptor de archivo

    return 0;
}
