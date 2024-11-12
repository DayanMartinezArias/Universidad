#include <iostream>
#include <unistd.h>      // Para fork(), getpid(), getppid(), sleep()
#include <sys/wait.h>    // Para wait()

void performHeavyTask() {
    std::cout << "[HIJO] Realizando tarea pesada..." << std::endl;
    sleep(5);  // Simula una tarea pesada con una espera de 5 segundos
    std::cout << "[HIJO] Tarea pesada completada." << std::endl;
}

void performOtherTask() {
    std::cout << "[PADRE] Realizando otra tarea en el proceso padre..." << std::endl;
    sleep(2);  // Simula otra tarea con una espera de 2 segundos
    std::cout << "[PADRE] Otra tarea en el proceso padre completada." << std::endl;
}

int main() {
    pid_t child = fork();  // Crear un proceso hijo

    if (child == 0) {
        // Código del proceso hijo
        std::cout << "[HIJO] Soy el proceso hijo con PID: " << getpid() << std::endl;
        performHeavyTask();
        return 0;  // El hijo termina su ejecución aquí
    }
    else if (child > 0) {
        // Código del proceso padre
        std::cout << "[PADRE] Soy el proceso padre con PID: " << getpid() << std::endl;
        performOtherTask();

        // Espera a que el proceso hijo termine
        int status;
        wait(&status);
        std::cout << "[PADRE] El proceso hijo ha terminado con el estado: " << WEXITSTATUS(status) << std::endl;
        std::cout << "[PADRE] Continuando con otras tareas después de que el hijo terminó." << std::endl;
    }
    else {
        // Error en fork()
        std::cerr << "Error al crear el proceso hijo." << std::endl;
        return 1;
    }

    return 0;
}
