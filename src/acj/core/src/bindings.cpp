#include <pybind11/pybind11.h>
#include "spatial_index.hpp"
#include "graph_simplify.hpp"

namespace py = pybind11;

PYBIND11_MODULE(acj_core, m) {
    m.doc() = "ACJ Core - CGAL-based spatial indexing & graph simplification";

    m.def("match_point", &match_point,
          "Encuentra el punto objetivo más cercano para cada punto de consulta.",
          py::arg("query_points"), py::arg("target_points"));

    m.def("find_clusters_cgal", &find_clusters_cgal,
          "Encuentra clústeres de puntos dentro de un umbral (O(N log N)).",
          py::arg("points"), py::arg("threshold"));

    m.def("match_segment", &match_segment,
          "Encuentra el segmento de línea más cercano para cada punto de consulta.",
          py::arg("query_points"), py::arg("segments"));

    m.def("simplify_graph_topological_cgal", &simplify_graph_topological_cgal,
          "Simplifica un grafo eliminando nodos de grado 2 (C++).",
          py::arg("nodes"), py::arg("segments"));

    m.def("simplify_graph_geometric_cgal", &simplify_graph_geometric_cgal,
          "Simplifica un grafo fusionando intersecciones cercanas (C++).",
          py::arg("nodes"), py::arg("segments"), py::arg("threshold"));

    m.def("simplify_graph_parallel_cgal", &simplify_graph_parallel_cgal,
          "Simplifica un grafo fusionando segmentos paralelos (e.g., doble calzada) (C++).",
          py::arg("nodes"), py::arg("segments"), py::arg("distance_threshold"), py::arg("angle_threshold_deg"));
	m.def("simplify_graph_minkowski_cgal", &simplify_graph_minkowski_cgal,
          "Simplifica un grafo usando Minkowski Buffers y Straight Skeleton vectorial (C++).",
          py::arg("nodes"), py::arg("segments"), py::arg("radius"));
}
