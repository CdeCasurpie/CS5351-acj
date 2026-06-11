#include <pybind11/pybind11.h>
#include "spatial_index.hpp"
#include "graph_simplify.hpp" // Asegúrate de que la firma de simplify_graph_acj_master_cgal esté en este header

namespace py = pybind11;

PYBIND11_MODULE(acj_core, m) {
    m.doc() = "ACJ Core - CGAL-based spatial indexing & graph simplification";

    py::class_<SimplificationResult>(m, "SimplificationResult")
        .def_readwrite("graph", &SimplificationResult::graph)
        .def_readwrite("node_lineage", &SimplificationResult::node_lineage)
        .def_readwrite("edge_lineage", &SimplificationResult::edge_lineage);


    // ==========================================================
    // 1. ÍNDICES ESPACIALES Y CLUSTERING
    // ==========================================================
    m.def("match_point", &match_point,
          "Encuentra el punto objetivo más cercano para cada punto de consulta.",
          py::arg("query_points"), py::arg("target_points"));

    m.def("find_clusters_cgal", &find_clusters_cgal,
          "Encuentra clústeres de puntos dentro de un umbral (O(N log N)).",
          py::arg("points"), py::arg("threshold"));

    m.def("match_segment", &match_segment,
          "Encuentra el segmento de línea más cercano mediante AABB Tree (O(N log M)).",
          py::arg("query_points"), py::arg("segments"));

    // ==========================================================
    // 2. MOTOR MAESTRO ACJ (ALGORITMO DEFINITIVO)
    // ==========================================================
    m.def("simplify_graph_acj_master_cgal", &simplify_graph_acj_master_cgal,
          "Motor Maestro Híbrido ACJ: Simplificación topológica vectorial y geométrica de Voronoi.",
          py::arg("nodes"), py::arg("segments"), 
          py::arg("angulo_maximo_desviacion"), 
          py::arg("factor_ajuste"), 
          py::arg("factor_epsilon"), 
          py::arg("with_index"));

    // ==========================================================
    // 3. ALGORITMOS EXPERIMENTALES / LEGACY (Para Benchmarks)
    // ==========================================================
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

    m.def("minkowski_guided_simplify", &minkowski_guided_simplify, 
          "Esqueletización guiada por topología",
          py::arg("nodes_x"), py::arg("nodes_y"), 
          py::arg("seg_start"), py::arg("seg_end"), py::arg("radius"));
}
