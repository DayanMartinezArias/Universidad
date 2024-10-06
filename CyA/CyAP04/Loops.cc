#include "Loops.h"

// Constructor
Loop::Loop(LoopTypes type, int line) : type_(type), line_(line) {}

// Getters
LoopTypes Loop::GetType() const {
    return type_;
}

int Loop::GetLine() const {
    return line_;
}

// Función para convertir LoopTypes a string
std::string LoopTypeToString(LoopTypes type) {
    switch (type) {
        case LoopTypes::For: 
          return "for";
        case LoopTypes::While: 
          return "while";
        default: 
          return "unknown";
    }
}

// [ Line 23] LOOP : for
// Sobrecarga del operador de inserción para imprimir el bucle
std::ostream& operator<<(std::ostream& os, const Loop& loop) {
    os << "[ Line " << loop.GetLine() << "] LOOP: " << LoopTypeToString(loop.GetType());
    return os;
}

