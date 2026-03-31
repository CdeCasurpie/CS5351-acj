"""Test cases for graph simplification (Topological, Geometric, Parallel, and Minkowski)."""
import os
import sys
import pandas as pd
import pytest

sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))
import acj

class TestGraphSimplification:
    def test_simplify_graph_topological_basic(self):
        nodes = pd.DataFrame({'node_id': [0, 1, 2, 3], 'x': [0.0, 10.0, 20.0, 30.0], 'y': [0.0, 0.0, 0.0, 0.0]})
        segments = pd.DataFrame({'segment_id': [0, 1, 2], 'node_start': [0, 1, 2], 'node_end': [1, 2, 3],
                                 'x1': [0.0, 10.0, 20.0], 'y1': [0.0, 0.0, 0.0], 'x2': [10.0, 20.0, 30.0], 'y2': [0.0, 0.0, 0.0]})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph_topological(graph)
        assert len(simplified.nodes) == 2
        assert len(simplified.segments) == 1

    def test_simplify_graph_topological_intersection_preserved(self):
        nodes = pd.DataFrame({'node_id': [0, 1, 2, 3, 4], 'x': [0.0, 10.0, 20.0, 10.0, 10.0], 'y': [0.0, 0.0, 0.0, 10.0, -10.0]})
        segments = pd.DataFrame({'segment_id': [0, 1, 2, 3], 'node_start': [0, 1, 1, 1], 'node_end': [1, 2, 3, 4],
                                 'x1': [0.0, 10.0, 10.0, 10.0], 'y1': [0.0, 0.0, 0.0, 0.0], 'x2': [10.0, 20.0, 10.0, 10.0], 'y2': [0.0, 0.0, 10.0, -10.0]})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph_topological(graph)
        assert len(simplified.nodes) == 5

    def test_simplify_graph_geometric_basic(self):
        nodes = pd.DataFrame({'node_id': [0, 1, 2, 3], 'x': [0.0, 5.0, 2.5, 2.5], 'y': [0.0, 0.0, 0.0, 10.0]})
        segments = pd.DataFrame({'segment_id': [0, 1, 2], 'node_start': [0, 1, 2], 'node_end': [2, 2, 3],
                                 'x1': [0.0, 5.0, 2.5], 'y1': [0.0, 0.0, 0.0], 'x2': [2.5, 2.5, 2.5], 'y2': [0.0, 0.0, 10.0]})
        graph = acj.load_graph(nodes, segments)
        simplified_merged = acj.simplify_graph_geometric(graph, threshold_meters=10.0)
        assert len(simplified_merged.nodes) == 2
        assert len(simplified_merged.segments) == 1

    def test_simplify_graph_empty_graph(self):
        nodes = pd.DataFrame({'node_id': [], 'x': [], 'y': []})
        segments = pd.DataFrame({'segment_id': [], 'node_start': [], 'node_end': [], 'x1': [], 'y1': [], 'x2': [], 'y2': []})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph(graph)
        assert len(simplified.nodes) == 0

    def test_simplify_graph_single_node(self):
        nodes = pd.DataFrame({'node_id': [0], 'x': [0.0], 'y': [0.0]})
        segments = pd.DataFrame({'segment_id': [], 'node_start': [], 'node_end': [], 'x1': [], 'y1': [], 'x2': [], 'y2': []})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph_topological(graph)
        assert len(simplified.nodes) == 1


class TestMinkowskiSimplification:
    """Test cases for the exact vectorial Minkowski Straight Skeleton algorithm."""

    def test_minkowski_empty_graph(self):
        nodes = pd.DataFrame({'node_id': [], 'x': [], 'y': []})
        segments = pd.DataFrame({'segment_id': [], 'node_start': [], 'node_end': [], 'x1': [], 'y1': [], 'x2': [], 'y2': []})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph(graph, method='minkowski')
        assert len(simplified.nodes) == 0
        assert len(simplified.segments) == 0

    def test_minkowski_parallel_merge(self):
        """Test that two parallel street segments close to each other merge into a single central street."""
        nodes = pd.DataFrame({'node_id': [0, 1, 2, 3], 'x': [0.0, 100.0, 0.0, 100.0], 'y': [0.0, 0.0, 4.0, 4.0]})
        segments = pd.DataFrame({'segment_id': [0, 1], 'node_start': [0, 2], 'node_end': [1, 3],
                                 'x1': [0.0, 0.0], 'y1': [0.0, 4.0], 'x2': [100.0, 100.0], 'y2': [0.0, 4.0]})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph(graph, threshold_meters=5.0, method='minkowski')
        assert len(simplified.segments) == 1
        assert len(simplified.nodes) == 2

    def test_minkowski_no_merge(self):
        """Test that segments far away from each other DO NOT merge."""
        nodes = pd.DataFrame({'node_id': [0, 1, 2, 3], 'x': [0.0, 100.0, 0.0, 100.0], 'y': [0.0, 0.0, 50.0, 50.0]})
        segments = pd.DataFrame({'segment_id': [0, 1], 'node_start': [0, 2], 'node_end': [1, 3],
                                 'x1': [0.0, 0.0], 'y1': [0.0, 50.0], 'x2': [100.0, 100.0], 'y2': [0.0, 50.0]})
        graph = acj.load_graph(nodes, segments)
        simplified = acj.simplify_graph(graph, threshold_meters=5.0, method='minkowski')
        assert len(simplified.segments) == 2
        assert len(simplified.nodes) == 4
