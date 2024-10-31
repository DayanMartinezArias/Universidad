/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo alfabeto.h
 * Contiene la declaración de la clase alfabeto
 * para ser usada por las cadenas y lenguajes, ya que ambas
 * tienen un respectivo alfabeto
 * Historial de revisiones
 * 17/09/2024 - Creación (primera versión) del código
 * 18/09/2024 - Creación de la clase y métodos útiles para trabajar con ella
 * 20/09/2024 - Creación de los operadores de igualdad y no igualdad, que serán usados en las otras clases
 * 21/09/2024 - Finalizado
 */

#ifndef ALFABETO_H
#define ALFABETO_H

#include <set>
#include <iostream>

class Alfabeto {
 public:
  Alfabeto() = default;
  Alfabeto(const std::string alfabeto);
  Alfabeto(const Alfabeto& alfabeto);
  friend std::ostream& operator<<(std::ostream& os, const Alfabeto& obj);
  friend std::istream& operator>>(std::istream& is, Alfabeto& obj);
  bool operator==(const Alfabeto& alfabeto) const;
  bool operator!=(const Alfabeto& alfabeto) const;
  bool ExisteSimbolo(const char simbolo) const;
  void InsertarSimbolo(const char simbolo);
  std::set<char> GetAlphabet() const;
  int GetCardinal();
  bool Vacio() const;
 private:
  std::set<char> alfabeto_;
};

#endif