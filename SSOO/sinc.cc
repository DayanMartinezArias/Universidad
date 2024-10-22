#include <iostream>
#include <thread>
#include <mutex>

int contador = 0;  // Recurso compartido
std::mutex mtx;    // Mutex para sincronización

void incrementar() {
    for (int i = 0; i < 1000; ++i) {
        std::lock_guard<std::mutex> guard(mtx);  // Bloquea el mutex
        ++contador;  // Incrementa el contador de manera segura
    }  // El mutex se libera automáticamente al salir del scope
}

int main() {
    std::thread hilo1(incrementar);
    std::thread hilo2(incrementar);

    hilo1.join();
    hilo2.join();

    std::cout << "Contador final (con sincronización): " << contador << std::endl;
    return 0;
}
