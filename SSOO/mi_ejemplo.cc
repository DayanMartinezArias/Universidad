#include <iostream>
#include <thread>
#include <string>
#include <mutex>

std::string mensaje{"Hola"};
std::mutex mtx;

void show_msg() {
  std::lock_guard<std::mutex> guard(mtx);
  std::cout << mensaje << std::endl;
}

int main () {
  std::thread hilo(show_msg);
  std::thread hilo2(show_msg);
  hilo.join();
  hilo2.join();
}