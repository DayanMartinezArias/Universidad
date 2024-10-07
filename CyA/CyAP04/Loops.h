/**
 * @file Loops.h
 * @author Martínez Arias, Dayánn alu0101644561
 * @brief Declaration of the Analizer class
 * @version 0.1
 * @date 2024-10-07
 */

#ifndef LOOPS_H
#define LOOPS_H

#include <string>
#include <iostream>

enum LoopTypes {
    For,
    While
};

class Loop {
 public:
  Loop(LoopTypes type, int line);
  LoopTypes GetType() const;
  int GetLine() const;
  friend std::ostream& operator<<(std::ostream& os, const Loop& loop);
 private:
  LoopTypes type_;  
  int line_;       
};

#endif
