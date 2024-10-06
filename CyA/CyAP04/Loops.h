#ifndef LOOPS_H
#define LOOPS_H

#include <string>
#include <iostream>

// Enum para los tipos de bucles
enum LoopTypes {
    For,
    While
};

class Loop {
 public:
  // Constructor
  Loop(LoopTypes type, int line);

  // Getters
  LoopTypes GetType() const;
  int GetLine() const;

  // Sobrecarga del operador de inserción para imprimir la información del bucle
  friend std::ostream& operator<<(std::ostream& os, const Loop& loop);

 private:
  LoopTypes type_;  // Tipo de bucle (For o While)
  int line_;        // Línea donde aparece el bucle
};

#endif
