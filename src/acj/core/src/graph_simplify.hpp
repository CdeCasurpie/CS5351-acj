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

py::tuple minkowski_guided_simplify(
    py::array_t<double> nodes_x,
    py::array_t<double> nodes_y,
    py::array_t<long> seg_start,
    py::array_t<long> seg_end,
    double radius
);

py::tuple simplify_graph_acj_master_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double angulo_maximo_desviacion,
    double factor_ajuste,
    double factor_epsilon,
    bool with_index
);
