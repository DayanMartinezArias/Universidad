#include <iomanip>  // Standard library headers first
#include "points.h"  // Project-specific header

namespace CyA {
// Overload for printing a point_vector
std::ostream& operator<<(std::ostream& os, const point_vector& ps) {
  os << ps.size() << std::endl;

  for (const point& p : ps) {
    os << p << std::endl;
  }

  return os;
}

// Overload for printing a single point
std::ostream& operator<<(std::ostream& os, const point& p) {
  os << std::setw(MAX_SZ) << std::fixed << std::setprecision(MAX_PREC) 
     << p.first << "\t" 
     << std::setw(MAX_SZ) << std::fixed << std::setprecision(MAX_PREC) 
     << p.second;

  return os;
}

// Overload for reading a point_vector
std::istream& operator>>(std::istream& is, point_vector& ps) {
  int n;
  is >> n;

  ps.clear();
  for (int i = 0; i < n; ++i) {
    point p;
    is >> p;
    ps.push_back(p);
  }

  return is;
}

// Overload for reading a single point
std::istream& operator>>(std::istream& is, point& p) {
  is >> p.first >> p.second;
  return is;
}

}  // namespace CyA
