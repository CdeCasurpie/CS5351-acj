import pytest
from acj import UrbanNetwork

def test_urban_network_initialization():
    network = UrbanNetwork()
    
    # Check that topological dataframes are properly initialized
    assert list(network.nodes_df.columns) == ['node_id', 'x', 'y']
    assert list(network.edges_df.columns) == ['segment_id', 'node_start', 'node_end']
    assert len(network.nodes_df) == 0
    assert len(network.edges_df) == 0
    
    # Check that semantic attributes are initialized as empty dicts
    assert network.node_metadata == {}
    assert network.edge_metadata == {}
    
    # Check lineage attributes
    assert network.lineage_nodes == {}
    assert network.lineage_edges == {}

def test_urban_network_repr():
    network = UrbanNetwork()
    repr_str = repr(network)
    
    # Check that it displays counts correctly
    assert "Nodes: 0" in repr_str
    assert "Edges: 0" in repr_str
    assert "Meta: False" in repr_str
    
    # Simulate adding metadata
    network.node_metadata[1] = {"highway": "residential"}
    repr_str_with_meta = repr(network)
    
    assert "Nodes: 0 (Meta: True)" in repr_str_with_meta
