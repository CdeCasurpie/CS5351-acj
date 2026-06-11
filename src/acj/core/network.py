import pandas as pd

class UrbanNetwork:
    """
    Core container for topological-semantic decoupling.
    Encapsulates topology for C++ operations and semantics for Python.
    """

    def __init__(self):
        # Topological Attributes (Strictly formatted for C++ invariance)
        self.nodes_df = pd.DataFrame(columns=['node_id', 'x', 'y'])
        self.edges_df = pd.DataFrame(columns=['segment_id', 'node_start', 'node_end'])

        # Semantic Attributes
        self.node_metadata = {}
        self.edge_metadata = {}

        # Lineage/Traceability Attributes
        self.lineage_nodes = {}
        self.lineage_edges = {}

    def __repr__(self) -> str:
        num_nodes = len(self.nodes_df)
        num_edges = len(self.edges_df)
        has_node_meta = len(self.node_metadata) > 0
        has_edge_meta = len(self.edge_metadata) > 0

        return (
            f"<UrbanNetwork | "
            f"Nodes: {num_nodes} (Meta: {has_node_meta}) | "
            f"Edges: {num_edges} (Meta: {has_edge_meta})>"
        )
