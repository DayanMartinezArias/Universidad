/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo alfabeto.cc
 * Contiene la implementación de la clase alfabeto
 * para ser usada por las cadenas y lenguajes, ya que ambas
 * tienen un respectivo alfabeto
 * Historial de revisiones
 * 17/09/2024 - Creación (primera versión) del código
 * 18/09/2024 - Creación de la clase y métodos útiles para trabajar con ella
 * 20/09/2024 - Creación de los operadores de igualdad y no igualdad, que serán usados en las otras clases
 * 21/09/2024 - Finalizado
 */

#include "alfabeto.h"

/**
 * @brief Construye un nuevo objeto de la clase alfabeto
 * 
 * @param alfabeto 
 */
Alfabeto::Alfabeto(const std::string alfabeto) {
  for (char simbolo : alfabeto) {
    alfabeto_.insert(simbolo);
  }
}

/**
 * @brief Constructor de copia
 * 
 * @param alfabeto 
 */
Alfabeto::Alfabeto(const Alfabeto& alfabeto) {
  alfabeto_ = alfabeto.alfabeto_;
}


/**
 * @brief Returns the the set of symbols of the alphabet
 * 
 * @return std::set<char> 
 */
std::set<char> Alfabeto::GetAlphabet() const {
  return alfabeto_;
}

/**
 * @brief Comprueba si existe un determinado símbolo en un alfabeto
 * 
 * @param simbolo 
 * @return true 
 * @return false 
 */
bool Alfabeto::ExisteSimbolo(const char simbolo) const {
  return (!(alfabeto_.find(simbolo) == alfabeto_.end()));
}

/**
 * @brief Inserta un nuevo simbolo en el alfabeto
 * 
 * @param simbolo 
 */
void Alfabeto::InsertarSimbolo(const char simbolo) {
  if (simbolo != ' ') alfabeto_.insert(simbolo);
}

/**
 * @brief Compara si dos alfabetos son iguales
 * 
 * @param alfabeto 
 * @return true 
 * @return false 
 */
bool Alfabeto::operator==(const Alfabeto& alfabeto) const{
  return alfabeto_ == alfabeto.alfabeto_;
}

/**
 * @brief Compara si dos alfabetos no son iguales
 * 
 * @param alfabeto 
 * @return true 
 * @return false 
 */
bool Alfabeto::operator!=(const Alfabeto& alfabeto) const {
  return alfabeto_ != alfabeto.alfabeto_;
}

/**
 * @brief Returns whether or not an alphabet is empty
 * 
 * @return true 
 * @return false 
 */
bool Alfabeto::Vacio() const {
  return alfabeto_.empty();
}

/**
 * @brief Crea el flujo de entrada
 * 
 * @param is 
 * @param obj 
 * @return std::istream& 
 */
std::istream& operator>>(std::istream& is, Alfabeto& obj) {
  std::string nuevo_alfabeto;
  is >> nuevo_alfabeto;

  Alfabeto aux;
  for (char simbolo : nuevo_alfabeto) {
    aux.InsertarSimbolo(simbolo);
  }
  obj = aux;
  return is;
}

/**
 * @brief Crea el flujo de salidaa
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Alfabeto& obj) {
  if (obj.alfabeto_.empty()) {
    os << "Alfabeto vacío, no es válido";
    return os;
  }
  os << "{";
  bool primero = true;  
  for (char simbolo : obj.alfabeto_) { 
    if (!primero) {
      os << ", "; 
    }
   os << simbolo;
   primero = false;  
  }
  os << "}";
  return os;
}



