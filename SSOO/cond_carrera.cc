#include <iostream>
#include <thread>

int contador = 0;  // Recurso compartido

void incrementar() {
    for (int i = 0; i < 1000; ++i) {
        ++contador;  // Incrementa el contador
    }
}

int main() {
    std::thread hilo1(incrementar);
    std::thread hilo2(incrementar);

    hilo1.join();
    hilo2.join();

    std::cout << "Contador final (sin sincronización): " << contador << std::endl;
    return 0;
}
