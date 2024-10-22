#include <iostream>
#include <thread>
#include <mutex>

class Contador {
private:
    int valor;
    std::mutex mtx;
    
public:
    Contador() : valor(0) {}
    
    void incrementar(int id, int cantidad) {
        for (int i = 0; i < cantidad; ++i) {
            std::lock_guard<std::mutex> guard(mtx);  // Bloquea el acceso al valor
            ++valor;
            std::cout << "Hilo " << id << " incrementa valor a " << valor << std::endl;
        }
    }
    
    int obtenerValor() const {
        return valor;
    }
};

int main() {
    Contador contador;
    
    std::thread hilo1(&Contador::incrementar, &contador, 1, 1000);  // Pasa función miembro como hilo
    std::thread hilo2(&Contador::incrementar, &contador, 2, 1000);
    
    hilo1.join();
    hilo2.join();
    
    std::cout << "Valor final del contador: " << contador.obtenerValor() << std::endl;
    return 0;
}
