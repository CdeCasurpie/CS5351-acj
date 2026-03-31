"""Test cases for MapIndex spatial queries."""
import os
import sys
import numpy as np
import pandas as pd
import pytest

sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))
import acj

class TestMapIndex:
    @pytest.fixture
    def simple_graph(self):
        nodes = pd.DataFrame({'node_id': [0, 1, 2, 3], 'x': [0.0, 100.0, 200.0, 100.0], 'y': [0.0, 0.0, 100.0, 100.0]})
        segments = pd.DataFrame({'segment_id': [0, 1, 2], 'node_start': [0, 1, 2], 'node_end': [1, 2, 3],
                                 'x1': [0.0, 100.0, 200.0], 'y1': [0.0, 0.0, 100.0], 'x2': [100.0, 200.0, 100.0], 'y2': [0.0, 100.0, 100.0]})
        return acj.load_graph(nodes, segments)

    def test_map_index_initialization(self, simple_graph):
        map_index = acj.MapIndex(simple_graph)
        assert map_index.graph_data is simple_graph
        assert map_index._endpoint_index is None
        assert map_index._acj_core is not None

    def test_assign_to_endpoints_simple(self, simple_graph):
        map_index = acj.MapIndex(simple_graph)
        points = pd.DataFrame({'point_id': [0, 1, 2], 'x': [5.0, 105.0, 195.0], 'y': [5.0, 5.0, 95.0]})
        result = map_index.assign_to_endpoints(points)
        assert 'assigned_node_id' in result.columns
        assert 'distance' in result.columns
        assert len(result) == len(points)
        assert result.loc[0, 'assigned_node_id'] == 0
        assert result.loc[1, 'assigned_node_id'] == 1
        assert result.loc[2, 'assigned_node_id'] == 2

    def test_assign_to_endpoints_missing_columns(self, simple_graph):
        map_index = acj.MapIndex(simple_graph)
        points = pd.DataFrame({'point_id': [0, 1], 'x': [5.0, 105.0]})
        with pytest.raises(ValueError, match="missing required columns"):
            map_index.assign_to_endpoints(points)

    def test_assign_to_endpoints_preserves_data(self, simple_graph):
        map_index = acj.MapIndex(simple_graph)
        points = pd.DataFrame({'point_id': [0, 1], 'x': [5.0, 105.0], 'y': [5.0, 5.0], 'crime_type': ['robbery', 'assault']})
        result = map_index.assign_to_endpoints(points)
        assert 'crime_type' in result.columns
        assert result.loc[0, 'crime_type'] == 'robbery'

    def test_assign_to_segments_not_implemented(self, simple_graph):
        map_index = acj.MapIndex(simple_graph)
        points = pd.DataFrame({'point_id': [0], 'x': [50.0], 'y': [0.0]})
        result = map_index.assign_to_segments(points)
        assert 'assigned_segment_id' in result.columns
        assert len(result) == 1
        assert result.loc[0, 'assigned_segment_id'] == 0
        result = map_index.assign_to_endpoints(points)

        assert 'crime_type' in result.columns
        assert result.loc[0, 'crime_type'] == 'robbery'
        assert result.loc[1, 'crime_type'] == 'assault'

    def test_assign_to_segments_not_implemented(self, simple_graph):
        """Test that assign_to_segments, though present in C++ core,
        is correctly flagged or handled in the Python wrapper API.
        We expect the python wrapper to call the C++ function and return a result.
        """
        map_index = acj.MapIndex(simple_graph)

        points = pd.DataFrame({
            'point_id': [0],
            'x': [50.0],
            'y': [0.0]
        })

        result = map_index.assign_to_segments(points)

        assert 'assigned_segment_id' in result.columns
        assert 'distance' in result.columns
        assert len(result) == 1
        assert result.loc[0, 'assigned_segment_id'] == 0
        assert np.isclose(result.loc[0, 'distance'], 0.0, atol=1e-6)

