#pragma once
#include "types.hpp"

SimplificationResult simplify_graph_topological_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments
);

SimplificationResult simplify_graph_geometric_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double threshold
);

SimplificationResult simplify_graph_parallel_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double distance_threshold,
    double angle_threshold_deg
);

SimplificationResult simplify_graph_minkowski_cgal(
	py::array_t<double> nodes,
	py::array_t<double> segments,
	double radius
);

SimplificationResult minkowski_guided_simplify(
    py::array_t<double> nodes_x,
    py::array_t<double> nodes_y,
    py::array_t<long> seg_start,
    py::array_t<long> seg_end,
    double radius
);

SimplificationResult simplify_graph_acj_master_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double angulo_maximo_desviacion,
    double factor_ajuste,
    double factor_epsilon,
    bool with_index
);
