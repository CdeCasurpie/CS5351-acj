#pragma once
#include "types.hpp"

py::tuple simplify_graph_topological_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments
);

py::tuple simplify_graph_geometric_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double threshold
);

py::tuple simplify_graph_parallel_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double distance_threshold,
    double angle_threshold_deg
);

py::tuple simplify_graph_minkowski_cgal(
	py::array_t<double> nodes,
	py::array_t<double> segments,
	double radius
);
