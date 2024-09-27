/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo cadena.h
 * Contiene la definición de la clase cadena
 * para poder ser realizados los métodos y funcionalidades propuestos en esta práctica.
 * Historial de revisiones
 * 17/09/2024 - Creación (primera versión) del código
 * 18/09/2024 - Creación de métodos y atributos básicos 
 * como la sobrecarga de operadores de insercción y extracción y los constructores
 * 19/09/2024 - Creación de métodos que se piden en la practica como el de invertir cadena
 * 20/09/2024 - Creación de los métodos sufijos y prefijos
 * 21/08/2024 - Aplicación de algunas mejoras, clase terminada.
 */

#ifndef CADENA_H
#define CADENA_H

#include <set>
#include "alfabeto.h"
#include "lenguaje.h"

class Lenguaje;

constexpr char kVacia{'&'};

class Cadena {
 public:
  Cadena();
  Cadena(const std::string& cadena, const Alfabeto& alfabeto);
  Cadena(const std::string& cadena);
  int GetLongitud() const;
  Alfabeto GetAlfabeto() const;
  Cadena Invertir() const;
  bool operator<(const Cadena& other) const;
  bool Vacia() const;
  bool AlfabetoValido() const;
  std::string GetSecuencia() const;
  Lenguaje Sufijos() const;
  Lenguaje Prefijos() const;
  friend std::ostream& operator<<(std::ostream& os, const Cadena& obj);
  friend std::istream& operator>>(std::istream& is, Cadena& obj);
 private:
  std::string cadena_;
  Alfabeto alfabeto_;
  int longitud_;
};

#endif