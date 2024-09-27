/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo lnguaje.cc
 * Contiene la clase Lenguaje
 * para poder ser realizados los métodos y funcionalidades propuestos en esta práctica.
 * Historial de revisiones
 */

#include "lenguaje.h"

/**
 * @brief Inserta una cadena en un lenguaje
 * 
 * @param cadena_entrada 
 */
void Lenguaje::InsertarCadena(const Cadena& cadena_entrada) {
  if (CadenaValida(cadena_entrada)) {
    lenguaje_.insert(cadena_entrada);
  }
}

/**
 * @brief Confirma si una cadena es válida
 * 
 * @param cadena_entrada 
 * @return true 
 * @return false 
 */
bool Lenguaje::CadenaValida(const Cadena& cadena_entrada) {
  return cadena_entrada.GetAlfabeto() == GetAlfabeto();
}

/**
 * @brief Devuelve el alfabeto de la cadena
 * 
 * @return Alfabeto 
 */
Alfabeto Lenguaje::GetAlfabeto() {
  return alfabeto_;
}

/**
 * @brief Sobrecarga del operador de extracción
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Lenguaje& obj) {
  os << "{";
  bool primero = true;  
  for (const Cadena& simbolo : obj.lenguaje_) { 
    if (!primero) {
      os << ", "; 
    }
   os << simbolo;
   primero = false;  
  }
  os << "}";
  return os;
}
