import os
import sys
import osmnx as ox

# Ensure acj package is accessible
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'src'))
import acj
from acj import UrbanNetwork, ACJTopologicalEvaluator, CompressionRatioMetric, SemanticSpeedDistortionMetric

def main():
    print("1. Descargando grafo de OSMnx (Barranco, Lima, Peru)...")
    # Using a small bounding box or place to ensure it runs quickly
    try:
        G = ox.graph_from_place("Barranco, Lima, Peru", network_type="drive")
    except Exception as e:
        print("Error descargando desde OSMnx. Fallback a un bounding box pequeño...")
        # Fallback coordinate box for Barranco
        G = ox.graph_from_bbox(north=-12.13, south=-12.15, east=-77.01, west=-77.03, network_type="drive")

    print("\n2. Parseando grafo a UrbanNetwork...")
    network = UrbanNetwork.from_networkx(G)
    print("Estado inicial del grafo:")
    print(network)
    
    print("\n3. Instanciando ACJTopologicalEvaluator...")
    metrics = [CompressionRatioMetric(), SemanticSpeedDistortionMetric()]
    evaluator = ACJTopologicalEvaluator(network, metrics)
    
    print("\n4. Ejecutando la simplificación y evaluación (esto puede tardar unos segundos)...")
    results = evaluator.evaluate()
    
    print("\n5. Resultados de Métricas:")
    for metric_name, value in results.items():
        print(f"   - {metric_name}: {value:.4f}")
        
    print("\n6. Trazabilidad Semántica:")
    simplified_network = evaluator.simplified_network
    print(simplified_network)
    
    if len(simplified_network.edge_metadata) > 0:
        sample_id = list(simplified_network.edge_metadata.keys())[0]
        metadata = simplified_network.edge_metadata[sample_id]
        print(f"\nEjemplo de fusión semántica en nueva arista {sample_id}:")
        for k, v in metadata.items():
            print(f"   - {k}: {v}")
    else:
        print("\nNo se encontraron metadatos en el grafo simplificado.")

if __name__ == "__main__":
    main()
