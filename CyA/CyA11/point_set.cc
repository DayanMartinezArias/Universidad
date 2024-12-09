#include <algorithm>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include "point_set.h"

point_set::point_set(const CyA::point_vector &points) : CyA::point_vector(points), emst_() {
  EMST();
}

void point_set::EMST(void) {
  CyA::arc_vector av;
  compute_arc_vector(av);

  forest st;

  for (const CyA::point &p : *this) {
    EMST::sub_tree s;
    s.add_point(p);
    st.push_back(s);
  }
  for (const CyA::weigthed_arc &a : av) {
    int i, j;
    find_incident_subtrees(st, a.second, i, j);
    if (i != j) {
      merge_subtrees(st, a.second, i, j);
    }
  }
   emst_ = st[0].get_arcs();
}

void point_set::compute_arc_vector(CyA::arc_vector &av) const {
  av.clear();
  const int n = size();
  for (int i = 0; i < n - 1; ++i) {
    const CyA::point &p_i = (*this)[i];
    for (int j = i + 1; j < n; ++j) {
      const CyA::point &p_j = (*this)[j];
      const double dist = euclidean_distance(std::make_pair(p_i, p_j));
      av.push_back(std::make_pair(dist, std::make_pair(p_i, p_j)));
    }
  }
 std::sort(av.begin(), av.end());
}

double point_set::euclidean_distance(const CyA::arc& a) const {
  const CyA::point& p1 = a.first;
  const CyA::point& p2 = a.second;
  return std::sqrt(std::pow(p1.first - p2.first, 2) + std::pow(p1.second - p2.second, 2));
}

void point_set::find_incident_subtrees(const forest& st, const CyA::arc &a, int& i, int& j) const {
  i = j = -1;
  for (int tag{0}; tag < st.size(); ++tag) {
    if (st[tag].contains(a.first)) {
      i = tag;
    } if (st[tag].contains(a.second)) {
      j = tag;
    } if (i != -1 && j != -1) {
      break;
    }
  }
}

void point_set::merge_subtrees(forest& st, const CyA::arc &a, int i, int j) const {
  st[i].merge(st[j], std::make_pair(euclidean_distance(a), a));
  st.erase(st.begin() + j);
}

double point_set::compute_cost(void) const {
  double total_cost{0.0};
  for (const CyA::arc& arcs : emst_) {
    total_cost += euclidean_distance(arcs);
  }
  return total_cost;
}

void point_set::write(std::ostream &os) const {
  os << "Points:" << "\n";
  for (const CyA::point& point : *this) {
    os << "(" << point.first << ", " << point.second << ")" << std::endl;
  }
}

void point_set::write_tree(std::ostream &os) const {
  os << "Minimum Tree:" << std::endl;
  for (const CyA::arc& arc : emst_) {
    auto it = std::find(begin(), end(), arc.first);
    int inx = it - begin();
    auto it2 = std::find(begin(), end(), arc.second);
    int inx2 = it2 - begin();
    os << "from point " << inx << "(" << arc.first.first << ", " << arc.first.second << 
    ") to point " << inx2 << "(" << arc.second.first << ", " << arc.second.second << ")"
    << " with cost: " << euclidean_distance(arc) << "\n";
  }
}

bool point_set::write_dot(const std::string& filename) const {
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << " for writing.\n";
    return false;
  }

  file << "graph {\n";


  for (int i{0}; i < size(); ++i) {
    file << "    " <<  i << " [pos=\"" << at(i).first << "," << at(i).second << "!\"];\n";
  }

  for (const CyA::arc& arcs : emst_) {
    auto it = std::find(begin(), end(), arcs.first);
    int inx = it - begin();
    file << "    " << inx << " -- "; 
    auto it2 = std::find(begin(), end(), arcs.second);
    int inx2 = it2 - begin();
    file << inx2 << " " << "\n";
  }

  file << "  " << "}";

  std::cout << "\nDOT file generated\n";

  return true;
}

