#include "point_set.h"
#include <algorithm>
#include <cmath>
#include <fstream>
    
void CyA::point_set::quickHull(void) {
  hull_.clear();

  CyA::point min_x_point; 
  CyA::point max_x_point;

  x_bounds(min_x_point, max_x_point);

  quickHull(CyA::line(min_x_point, max_x_point), side::LEFT);
  quickHull(CyA::line(min_x_point, max_x_point), side::RIGHT);

  // Remove duplicates
  std::sort(hull_.begin(), hull_.end());
  hull_.erase(std::unique(hull_.begin(), hull_.end()), hull_.end());
}

void CyA::point_set::quickHull(const CyA::line &l, int side) {
  CyA::point farthest;

  if (farthest_point(l, side, farthest)) {
    quickHull(CyA::line(l.first, farthest), - find_side(CyA::line(l.first, farthest), l.second));
    quickHull(CyA::line(farthest, l.second), - find_side(CyA::line(farthest, l.second), l.first));
  }
  else {
    hull_.push_back(l.first);
    hull_.push_back(l.second);
  }
}

bool CyA::point_set::farthest_point(const CyA::line &l, int side, CyA::point &farthest) const {
  farthest = CyA::point_vector::at(0);

  double max_dist = 0;

  bool found = false;

  for (const CyA::point &p : *this) {
    const double dist = distance(l, p);

    if (find_side(l, p) == side && dist > max_dist) {
      farthest = p;
      max_dist = dist;
      found = true;
    }
  }
  return found;
}

double CyA::point_set::point_2_line(const CyA::line &l, const CyA::point &p) const {
  const CyA::point &p1 = l.first;
  const CyA::point &p2 = l.second;

  return (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first);
}

double CyA::point_set::distance(const CyA::line &l, const CyA::point &p) const {
  return std::fabs(point_2_line(l, p));
}

int CyA::point_set::find_side(const CyA::line &l, const CyA::point &p) const {
  double result = point_2_line(l, p);
  if (result > 0) return side::RIGHT;
  if (result < 0) return side::LEFT;
  return side::CENTER;
}

void CyA::point_set::x_bounds(CyA::point &min_x, CyA::point &max_x) const {
  min_x = max_x = CyA::point_vector::at(0);
  
  for (const CyA::point &p : *this) {
    if (p.first < min_x.first || (p.first == min_x.first && p.second < min_x.second)) {
      min_x = p;
    }
    if (p.first > max_x.first || (p.first == max_x.first && p.second > max_x.second)) {
      max_x = p;
    }
  }
}

void CyA::point_set::write(std::ostream &os) const {

}

void CyA::point_set::write_hull(std::ostream &os) const {
  for (const CyA::point &p : hull_) {
    os << "(" << p.first << ", " << p.second << ")\n";
  }
}

bool CyA::point_set::write_dot(const std::string& filename) const {
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << " for writing.\n";
    return false;
  }

  file << "graph {\n";


  for (int i{0}; i < size(); ++i) {
    file << "    " <<  i << " [pos=\"" << at(i).first << "," << at(i).second << "!\"];\n";
  }
  
    const point_vector& hull = get_hull();
  for (size_t i = 0; i < hull.size(); ++i) {
    // Find indices of points in the hull
    int index1 = std::distance(begin(), std::find(begin(), end(), hull[i]));
    int index2 = std::distance(begin(), std::find(begin(), end(), hull[(i + 1) % hull.size()]));
    file << "    " << index1 << " -- " << index2 << ";\n";
  }

  file << "  " << "}";

  std::cout << "\nDOT file generated\n";

  return true;
}
