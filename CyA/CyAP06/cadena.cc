/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo cadena.cc
 * Contiene la implementación de clase cadena
 * para poder ser realizados los métodos y funcionalidades propuestos en esta práctica.
 * Historial de revisiones
 * 17/09/2024 - Creación (primera versión) del código
 * 18/09/2024 - Creación de métodos y atributos básicos 
 * como la sobrecarga de operadores de insercción y extracción y los constructores
 * 19/09/2024 - Creación de métodos que se piden en la practica como el de invertir cadena
 * 20/09/2024 - Creación de los métodos sufijos y prefijos
 * 21/08/2024 - Aplicación de algunas mejoras, clase terminada.
 */

#include "cadena.h"
#include <algorithm>
#include <stdexcept>

/**
 * @brief Constructor por defecto
 * 
 */
Cadena::Cadena() : cadena_(std::string(1, kVacia)), longitud_(0) {
  alfabeto_ = Alfabeto("0"); // Usa el símbolo por defecto
}

/**
 * @brief Constructor de la cadena
 * 
 * @param cadena 
 * @param alfabeto 
 */
Cadena::Cadena(const std::string& cadena, const Alfabeto& alfabeto) : 
cadena_(cadena), alfabeto_(alfabeto), longitud_(cadena == std::string(1, kVacia) ? 0 : cadena.length()) {}

/**
 * @brief Constructor sin alfabeto
 * 
 * @param cadena 
 */
Cadena::Cadena(const std::string& cadena) : cadena_(cadena), longitud_(cadena == std::string(1, kVacia) ? 0 : cadena.length()) {
  if (cadena == std::string(1, kVacia)) {
    Alfabeto aux("0");
    alfabeto_ = aux;
  } else {
    Alfabeto aux(cadena);
    alfabeto_ = aux;
  }
}

/**
 * @brief Sobrecarga del operador<, útil al ordenar cadenas
 * 
 * @param other 
 * @return true 
 * @return false 
 */
bool Cadena::operator<(const Cadena& other) const {
  if (GetSecuencia().length() != other.GetSecuencia().length()) {
    return GetSecuencia().length() < other.GetSecuencia().length();
  }
  return GetSecuencia() < other.GetSecuencia(); 
}

/**
 * @brief Devuelve la secuencia en forma de std::string de la cadena
 * 
 * @return std::string 
 */
std::string Cadena::GetSecuencia() const {
  return cadena_;
}

/**
 * @brief Se asegura si una cadena está vacía
 * 
 * @return true 
 * @return false 
 */
bool Cadena::Vacia() const {
  return GetSecuencia() == std::string(1, kVacia);
}

/**
 * @brief Devuelve la longitud de la cadena
 * 
 * @return int 
 */
int Cadena::GetLongitud() const {
  return longitud_;
}

/**
 * @brief Devuelve el alfabeto de la cadena
 * 
 * @return Alfabeto 
 */
Alfabeto Cadena::GetAlfabeto() const {
  return alfabeto_;
}

/**
 * @brief Sobrecarga del operador de extracción
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Cadena& obj) {
  if (obj.longitud_ == 0) {
    os << kVacia;
    return os;
  } 
  for (char simbolo : obj.GetSecuencia()) {
    os << simbolo;
  }
  return os;
}

/**
 * @brief Se asegura si un alfabeto es válido para una cadena
 * 
 * @param alfabeto 
 * @return true 
 * @return false 
 */
bool Cadena::AlfabetoValido() const {
  if (Vacia()) {
    return true;
  }
  for (char simbolo : GetSecuencia()) {
    if (!alfabeto_.ExisteSimbolo(simbolo)) {
      return false; 
    }
  }
  return true;
}

/**
 * @brief Sobrecarga del opeardor de insercción
 * 
 * @param is 
 * @param obj 
 * @return std::istream& 
 */
std::istream& operator>>(std::istream& is, Cadena& obj) {
  std::string nueva_cadena, nuevo_alfabeto;
  Alfabeto aux;
   
  is >> nueva_cadena >> nuevo_alfabeto;
  
  // Si la cadena es vacía
  if (nueva_cadena == std::string(1, kVacia) && nuevo_alfabeto.empty()) {
    obj.cadena_ = nueva_cadena;
    aux.InsertarSimbolo('0');  
    obj.alfabeto_ = aux;
    obj.longitud_ = 0;
    return is;
  }

  if (nuevo_alfabeto.empty()) {
    nuevo_alfabeto = nueva_cadena;
  }

  for (char simbolo : nuevo_alfabeto) {
    aux.InsertarSimbolo(simbolo);
  }

  obj.cadena_ = nueva_cadena;
  obj.alfabeto_ = aux;
  obj.longitud_ = nueva_cadena == "&" ? 0 : nueva_cadena.length();

  return is;
}


/**
 * @brief Devuelve una cadena a partir de la original invertida
 * 
 * @return Cadena 
 */
Cadena Cadena::Invertir() const {
  std::string inversa{GetSecuencia()};
  std::reverse(inversa.begin(), inversa.end());
  return Cadena(inversa, alfabeto_);
} 


