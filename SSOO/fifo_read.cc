#include <iostream>
#include <fcntl.h>    // Para open()
#include <unistd.h>   // Para read()

int main() {
    const char* path = "./mi_fifo";
    int fd = open(path, O_RDONLY);  // Abrir el FIFO en modo lectura
    
    char buffer[100];
    read(fd, buffer, sizeof(buffer));  // Leer del FIFO
    std::cout << "Mensaje recibido: " << buffer << std::endl;
    close(fd);  // Cerrar el descriptor de archivo

    return 0;
}
