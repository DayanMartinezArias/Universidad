#include "point_set.h"
#include <iostream>
#include <sstream>
#include <vector>

int main() {
  std::vector<CyA::point> points;
  double x, y;

  std::cout << "Enter points as pairs of x y (e.g., 1.0 2.0).\n";

  // Read points from standard input
  while (std::cin >> x >> y) {
    points.emplace_back(x, y);
  }

  if (points.empty()) {
    std::cerr << "No points provided. Exiting.\n";
    return 1;
  }

  // Create a point_set object with the input points
  CyA::point_set pointSet(points);

  // Compute the convex hull
  pointSet.quickHull();

  // Output the results
  std::cout << "\nOriginal Points:\n";
  pointSet.write(std::cout);

  std::cout << "\nConvex Hull Points:\n";
  pointSet.write_hull(std::cout);

  if (pointSet.write_dot("out.dot")) {
    std::cout << "DOT file generated\n";
  }

  return 0;
}
