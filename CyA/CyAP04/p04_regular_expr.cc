/**
 * @file p04_regular_expr.cc
 * @author Martínez Arias, Dayán alu0101644561
 * @brief Main file, controls things like parameters, helps the user and performs the main purpose of the program
 * @version 0.1
 * @date 2024-10-07
 * 
 */

#include "Comments.h"
#include "Loops.h"
#include "Variable.h"
#include "Analizer.h"
#include "reader.h"
#include <string>
#include <regex>

constexpr int kCorrectParams{3};

/**
 * @brief Valida los parametros de entrada
 * 
 * @param params 
 * @return true 
 * @return false 
 */
bool CheckCorrectParams(const int params) {
  if ((params != kCorrectParams)) {
    std::cerr << "Número incorrecto de parametros" << std::endl;
    std::cerr << "Introduzca " << kCorrectParams << " parametros" << std::endl;
    std::cerr << "Para más información ejecute: p04_regular_expr --help" << std::endl;
    return false;
  } 
  return true;
}

/**
 * @brief Informa al usuario del uso del programa
 * 
 */
void Uso() {
  std::cout << "Modo de empleo: p04_regular_expr filein.txt fileout.txt" << std::endl;
  std::cout << "Pruebe p04_regular_expr --help para más información." << std::endl;
}

/**
 * @brief Se ejecuta cuando el usuario ejecuta el programa con --help
 * 
 */
void Ayuda() {
  std::cout << "EXPRESIONES REGULARES" << std::endl;
  Uso();
  std::cout << "Toma un fichero de código y devuelve una salida con datos resumidos del código" << std::endl;
}

/**
 * @brief Main function of the program
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main (int argc, char* argv[]) {
  if(argc == 1) {
    Uso();
    return 0;
  }
  std::string arg1{argv[1]};
  if (arg1 == "--help") {
    Ayuda();
    return 0;
  }
  if (!CheckCorrectParams(argc)) {
    return 1;
  }
  FileReader in_file(argv[1]);
  std::vector<std::string> vector_of_lines = in_file.ReadFile();
  Analyzer file_analyzer(vector_of_lines, argv[1]);
  file_analyzer.Analyze();
  std::ofstream out_file(argv[2]);
  out_file << file_analyzer;
  out_file.close();
  return 0;
}