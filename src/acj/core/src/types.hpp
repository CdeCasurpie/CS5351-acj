#pragma once

#include <cmath>
#include <limits>
#include <vector>
#include <map>
#include <functional>
#include <utility>
#include <tuple>
#include <set>
#include <deque>
#include <algorithm>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// Simplificacion del grafo con suma de minkowski, union booleana y staright Straight_skeleton_2
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/Straight_skeleton_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                   DT;
typedef K::Point_2                                          Point_pt;
typedef K::Segment_2                                        Segment_k;
typedef K::Vector_2                                         Vector_k;
typedef CGAL::Exact_predicates_exact_constructions_kernel Exact_K;
typedef CGAL::Polygon_2<Exact_K>                          Polygon_exact;
typedef CGAL::Polygon_with_holes_2<Exact_K>               Polygon_with_holes_exact;
typedef CGAL::Polygon_set_2<Exact_K>                      Polygon_set_exact;
typedef CGAL::Straight_skeleton_2<Exact_K>                Straight_skeleton;
typedef std::shared_ptr<Straight_skeleton>                Straight_skeleton_ptr;
typedef std::tuple<long, long, long, Point_pt, Point_pt, Segment_k> SegmentInfo;

namespace py = pybind11;

typedef std::map<long, Point_pt> NodeCoordMap;
typedef std::map<long, int> NodeDegreeMap;
typedef std::map<long, std::vector<long>> AdjacencyMap;
