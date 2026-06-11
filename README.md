# ACJ: Urban Graph Acceleration & Simplification Framework

![Python](https://img.shields.io/badge/Python-3.10+-blue.svg?logo=python&logoColor=white)
![C++](https://img.shields.io/badge/C++-17-00599C.svg?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.15+-064F8C.svg?logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green.svg)

**ACJ** is a high-performance hybrid framework (C++/Python) designed for the semantic and topological simplification of large-scale urban networks. It safely decouples complex geometric operations from semantic data attributes, ensuring that metadata (e.g., speed limits, road names) survives massive graph reductions.

## System Architecture

The core philosophy of ACJ relies on treating `UrbanNetwork` as an invariant Registry/Proxy. Heavy geometric simplifications are sent to a highly optimized C++ core using CGAL. The core returns the simplified topology alongside strict lineage maps, which Python then uses to reconstruct and resolve semantic collisions.

```mermaid
graph TD
    subgraph Inputs ["Multimodal Inputs"]
        A1[OSMnx]
        A2[DataFrames]
        A3[Shapefiles]
    end

    A1 -->|from_networkx| B
    A2 -->|from_dataframe| B
    A3 -->|Parsers| B

    subgraph Python ["Python Layer"]
        B[UrbanNetwork <br/> Registry/Proxy]
        E[Semantic Resolution <br/> Metadata Fusion]
        F[Metrics & Evaluator]
    end

    subgraph Cpp ["C++ Core (CGAL/pybind11)"]
        C[Raw Topology <br/> V, E matrices]
        D[Geometric & Topological <br/> Simplification Algorithms]
    end

    B -.->|Passes Topology| C
    C --> D
    D -.->|SimplificationResult <br/> Topology + Lineage Maps| E
    B -.->|Passes Metadata| E

    E -->|Resolved UrbanNetwork| F
```

## Installation Guide

### Prerequisites
You need a system with C++17 support, CMake, and the following libraries:
- **CGAL** (Computational Geometry Algorithms Library)
- **Boost**
- **pybind11**

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install cmake libcgal-dev libboost-all-dev
```
On Arch Linux:
```bash
sudo pacman -S cmake cgal boost pybind11
```

### Building the Package
We highly recommend setting up a virtual environment:

```bash
python -m venv venv
source venv/bin/activate
pip install -U pip setuptools wheel
```

Install the package in editable mode (which will trigger CMake build):

```bash
pip install -e .
```

## Quick Start (E2E Pipeline)

This demonstrates the end-to-end pipeline: extracting raw data, parsing, executing a highly optimized topological simplification, and calculating compression metrics.

```python
import osmnx as ox
from acj import UrbanNetwork, ACJTopologicalEvaluator
from acj import CompressionRatioMetric, SemanticSpeedDistortionMetric

# 1. Ingest Data (Raw Graph)
G = ox.graph_from_place("Barranco, Lima, Peru", network_type="drive")

# 2. Parse into Multimodal Registry
network = UrbanNetwork.from_networkx(G)
print("Initial:", network)

# 3. Setup Metrics and Evaluator
metrics = [CompressionRatioMetric(), SemanticSpeedDistortionMetric()]
evaluator = ACJTopologicalEvaluator(network, metrics)

# 4. Evaluate (C++ Topo Simplification + Semantic Resolution)
results = evaluator.evaluate()

print("Results:", results)
print("Simplified:", evaluator.simplified_network)
```

## Module Structure (API Reference)

- `acj.core`: Contains the fundamental proxy data structures (`UrbanNetwork`) and the semantics collision resolver (`resolve_semantics`).
- `acj.algorithms`: Houses the pure Python geometric logic wrappers and spatial spatial map indices (`MapIndex`).
- `acj.data`: Input/Output handlers and data object bindings (`SimplificationResult`, `GraphData`).
- `acj.evaluation`: Evaluation engine, concrete metric classes, and evaluators (`BaseEvaluator`, `ACJTopologicalEvaluator`).
