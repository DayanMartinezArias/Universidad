#include <iostream>
#include <string>
#include <utility>

constexpr double PORCION_HC{10.0};
std::pair<double, double> CalcularGramos(const double& HC, const double& por_cada) {
  double porciones = HC / PORCION_HC;
  double gramos_del_producto = (PORCION_HC * por_cada) / HC;
  return std::make_pair(porciones, gramos_del_producto);
}


int main (int argc, char* argv[]) {
  std::pair<double, double> gramos = CalcularGramos(std::stod(argv[1]), std::stod(argv[2]));
  std::cout << "Por cada " << argv[2] << " gramos de producto hay " << gramos.first << " porciones" << "\n";
  std::cout << "Necesitas " << gramos.second << " unidades del producto para cumplir con una porcion" << "\n";
}