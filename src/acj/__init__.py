"""
ACJ - Advanced Geospatial Analysis Library

A Python library for efficient geospatial point-to-graph assignment using CGAL.
Designed for large-scale urban analytics, crime mapping, and network analysis.

Main Components:
    - MapIndex: Core class for spatial indexing and queries
    - load_graph: Load graph data from pandas DataFrames
    - simplify_graph: Graph simplification utilities
    
Author: Cèsar, Alejandro, Jerimy.
"""

# Importaciones con las nuevas rutas de Clean Architecture
from .algorithms.map_index import MapIndex
from .data.io import load_graph, load_map, GraphData
from .algorithms.graph import simplify_graph, simplify_graph_topological, simplify_graph_geometric, simplify_graph_parallel_cgal
from .utils.render import render_graph, render_heatmap, render_comparison

__version__ = "0.1.0"
__all__ = [
    "MapIndex",
    "load_graph",
    "load_map",
    "GraphData",
    "simplify_graph",
    "simplify_graph_topological",
    "simplify_graph_geometric",
    "simplify_graph_parallel_cgal",
    "render_graph",
    "render_heatmap",
    "render_comparison",
]
