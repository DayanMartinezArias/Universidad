#pragma once

#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <set>

#define MAX_SZ 3
#define MAX_PREC 0

namespace CyA
{
    typedef std::pair<double, double> point; // a single point in a 2d space

    typedef std::pair<point, point> line; // a single line represented by two points
    typedef std::vector<point> point_vector; // a vector of multple points
    
    // Graph vars
    typedef std::pair<point, point> arc; // an arc of a graph
    typedef std::pair<double, arc> weigthed_arc; // a weighted arc, represented by the arc and its weight
    typedef std::vector<weigthed_arc> arc_vector; // a vector of arcs

    typedef std::set<point> point_collection; // a set of points

    typedef std::vector<arc> tree; // a tree, represented by a vector of arcs
}

std::ostream& operator<<(std::ostream& os, const CyA::point& p); // Declaración
std::ostream& operator<<(std::ostream& os, const CyA::point_vector& ps);
std::istream& operator>>(std::istream& is, CyA::point& p);
std::istream& operator>>(std::istream& is, CyA::point_vector& ps);
