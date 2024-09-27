/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo lnguaje.h
 * Contiene la clase Lenguaje
 * para poder ser realizados los métodos y funcionalidades propuestos en esta práctica.
 * Historial de revisiones
 */

#ifndef LENGUAJE_H
#define LENGUAJE_H

#include "cadena.h"

class Cadena;

class Lenguaje {
 public:
  Lenguaje() = default;
  Lenguaje(const Alfabeto& alfabeto) : alfabeto_(alfabeto) {};
  void InsertarCadena(const Cadena& cadena_entrada);
  Alfabeto GetAlfabeto();
  friend std::ostream& operator<<(std::ostream& os, const Lenguaje& obj);
 private:
   bool CadenaValida(const Cadena& cadena_entrada);

   std::set<Cadena> lenguaje_;
   Alfabeto alfabeto_;
};

#endif