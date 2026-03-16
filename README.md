# ACJ - Advanced Crime-to-Junction Assignment Library

A high-performance Python library for geospatial analysis and real-time visualization of point-based events on street networks. Combines CGAL computational geometry algorithms with GPU-accelerated rendering for large-scale urban analytics.

## Overview

ACJ provides efficient spatial assignment of point events (crimes, incidents, service requests) to street network nodes using Delaunay triangulation, with real-time interactive visualization capabilities.

**Key Features:**
- Fast spatial queries using CGAL Delaunay triangulation (6.1x faster than brute-force)
- Automatic street network loading from OpenStreetMap via OSMnx
- GPU-accelerated real-time visualization with VisPy/OpenGL
- Standardized pandas DataFrame interface
- Docker-based reproducible environment

**Performance:** Process 100,000 point assignments on 50,000 nodes in 1.3 seconds

**Based on work by:** Alejandro (CGAL integration and spatial indexing)

---

## Installation

This project requires compiling C++ extensions (CGAL) alongside Python. To make this painless, we provide three installation methods depending on your environment and expertise.

### 1. NixOS Flakes (Recommended for Developers)
If you use Nix, this is the most robust method. It provides a fully deterministic, container-free development environment with all C++ and Python dependencies pre-configured.

```bash
git clone <repository-url>
cd <repository-folder>

# 1. Enter the deterministic development environment
nix develop

# 2. Install the Python package in editable mode
pip install -e .

# 3. Compile the C++ extension (CGAL/pybind11)
mkdir build && cd build
cmake .. && make -j$(nproc)

# 4. Add the compiled binary to your Python path
export PYTHONPATH=$(pwd)/src/acj/core:$PYTHONPATH
```

### 2. Docker (Recommended for cross-platform users)

If you are on Windows, macOS, or standard Linux and want a "zero-setup" experience, use our Docker containers.

```bash
git clone <repository-url>
cd <repository-folder>

# 1. Build the Docker image with all dependencies
make build

# 2. Verify the installation by running the test suite
make test

# 3. Run interactive visualization (Linux requires X11 forwarding)
xhost +local:docker
make example-realtime

```

### 3. Manual Installation (Standard Linux)

If you prefer a traditional setup on Ubuntu/Debian, follow these steps to install system dependencies, set up a Python virtual environment, and compile the code.

```bash
# 1. Install system dependencies (C++, CMake, CGAL, Qt)
sudo apt-get update
sudo apt-get install build-essential cmake python3-dev python3-pip python3-venv \
    libcgal-dev pybind11-dev libspatialindex-dev libgeos-dev libproj-dev \
    libgl1-mesa-glx python3-pyqt5

# 2. Create and activate a Python virtual environment
python3 -m venv .venv
source .venv/bin/activate

# 3. Install Python dependencies and the package itself
pip install -r requirements.txt
pip install -e .

# 4. Compile the C++ extension
mkdir build && cd build
cmake .. && make -j$(nproc)

# 5. Export PYTHONPATH so Python finds the compiled .so file
export PYTHONPATH=$(pwd)/src/acj/core:$PYTHONPATH

```

## Quick Start

### Basic Usage

```python
import pandas as pd
from acj.data.io import load_graph
from acj.algorithms.map_index import MapIndex

# Load graph from DataFrames
nodes = pd.DataFrame({
    'node_id': [0, 1, 2],
    'x': [0.0, 100.0, 200.0],
    'y': [0.0, 0.0, 100.0]
})

segments = pd.DataFrame({
    'segment_id': [0, 1],
    'node_start': [0, 1],
    'node_end': [1, 2],
    'x1': [0.0, 100.0],
    'y1': [0.0, 0.0],
    'x2': [100.0, 200.0],
    'y2': [0.0, 100.0]
})

graph = load_graph(nodes, segments)

# Create spatial index
map_index = MapIndex(graph)

# Assign points to nearest nodes
crimes = pd.DataFrame({
    'point_id': [0, 1, 2],
    'x': [50.0, 150.0, 25.0],
    'y': [5.0, 10.0, 90.0],
    'crime_type': ['robbery', 'assault', 'theft']
})

assignments = map_index.assign_to_endpoints(crimes)
print(assignments[['point_id', 'assigned_node_id', 'distance']])

```

### Real City Example

```python
import numpy as np
import pandas as pd
from acj.data.io import load_map
from acj.algorithms.map_index import MapIndex
from acj.utils.render import render_heatmap

# Load street network from OpenStreetMap
graph = load_map("Cholula, Puebla, Mexico")

# Generate sample points
np.random.seed(42)
n = 1000
x_min, x_max = graph.nodes['x'].min(), graph.nodes['x'].max()
y_min, y_max = graph.nodes['y'].min(), graph.nodes['y'].max()

crimes = pd.DataFrame({
    'point_id': range(n),
    'x': np.random.uniform(x_min, x_max, n),
    'y': np.random.uniform(y_min, y_max, n)
})

# Assign to street network
map_index = MapIndex(graph)
assignments = map_index.assign_to_endpoints(crimes)

# Launch interactive visualization
render_heatmap(map_index, assignments, title="Crime Density Heatmap")

```

**Visualization Controls:**

* Mouse drag: Pan view
* Mouse wheel: Zoom
* N key: Toggle node visibility
* R key: Reset camera
* Q or ESC: Close window

---

## Data Format

All inputs and outputs use pandas DataFrames with standardized schemas.

### Nodes DataFrame (Input)

```python
pd.DataFrame({
    'node_id': int,    # Unique identifier
    'x': float,        # X coordinate (meters, projected CRS)
    'y': float         # Y coordinate (meters, projected CRS)
})

```

### Segments DataFrame (Input)

```python
pd.DataFrame({
    'segment_id': int,     # Unique identifier
    'node_start': int,     # Start node_id
    'node_end': int,       # End node_id
    'x1': float, 'y1': float,  # Start coordinates
    'x2': float, 'y2': float   # End coordinates
})

```

### Points DataFrame (Input)

```python
pd.DataFrame({
    'point_id': int,   # Unique identifier
    'x': float,        # X coordinate
    'y': float         # Y coordinate
    # Additional columns preserved in output
})

```

### Assignments DataFrame (Output)

```python
pd.DataFrame({
    'point_id': int,           # From input
    'x': float, 'y': float,    # From input
    'assigned_node_id': int,   # Nearest node
    'distance': float,         # Euclidean distance (meters)
    # All input columns preserved
})

```

**Important:** Coordinates must be in a projected coordinate system (e.g., UTM) in meters, not latitude/longitude.

---

## API Reference

### Data Loading (`acj.data.io`)

**`load_graph(nodes_df, segments_df)`**

Load a graph from pandas DataFrames.

Returns: `GraphData` object

---

**`load_map(city_name, cache_dir="./cache", network_type="drive")`**

Download a street network from OpenStreetMap.

Parameters:
- `city_name` (str): City query string (e.g., "Manhattan, New York")
- `cache_dir` (str): Cache directory path
- `network_type` (str): Network type ("drive", "walk", "bike", "all")

Returns: `GraphData` object

Example:
```python
from acj.data.io import load_map
graph = load_map("Cholula, Puebla, Mexico")

```

---

### Spatial Indexing (`acj.algorithms.map_index`)

**`MapIndex(graph_data)`**

Create a CGAL-based spatial index.

**Methods:**

**`assign_to_endpoints(points_df)`**

Assign points to the nearest graph nodes.

Complexity: `O(M log M)` build + `O(N log M)` query

Returns: DataFrame with assignments and distances

Example:

```python
from acj.algorithms.map_index import MapIndex
map_index = MapIndex(graph)
assignments = map_index.assign_to_endpoints(crimes)

```

**`get_render_data(assignments=None)`**

Pre-compute GPU render buffers.

Returns: Dictionary with vertex/color arrays

---

### Visualization (`acj.utils.render`)

**`render_heatmap(map_index, assignments, title="Crime Density Heatmap")`**

Launch an interactive heatmap visualization.

Features:

* Color gradient: white (low) to yellow to orange to red (high)
* Gradient interpolation on street segments
* Real-time zoom/pan with OpenGL acceleration
* Node visibility toggle

Blocks execution until the window is closed.

---

**`render_graph(map_index, title="Street Network")`**

Launch basic network visualization without heatmap coloring.

---

### Graph Utilities (`acj.algorithms.graph`)

**`simplify_graph(graph_data, threshold_meters=10.0)`**

Simplify the graph using automatic method selection.

* `threshold_meters = 0`: Topological simplification only
* `threshold_meters > 0`: Geometric simplification with distance threshold

**`simplify_graph_topological(graph_data)`**

Simplify the graph by removing degree-2 nodes (topological simplification).

* Preserves all intersections and connectivity
* Fast `O(n)` algorithm suitable for real-time use
* Best for maintaining network topology

**`simplify_graph_geometric(graph_data, threshold_meters=10.0)`**

Simplify the graph by merging nearby intersections using CGAL clustering.

* More aggressive simplification than topological
* May change network topology slightly
* Best for dense networks with many close intersections
* Uses CGAL Delaunay triangulation for high-performance spatial clustering

---

## Examples

### example_acj.py

Basic demonstration with synthetic data.

```bash
make example

```

Features:

* Graph creation from DataFrames
* Spatial index construction
* Point assignment
* Result statistics

---

### example_realtime.py

Complete pipeline with real city data.

```bash
xhost +local:docker
make example-realtime

```

Features:

* OpenStreetMap data loading
* Random point generation
* CGAL spatial assignment
* GPU-accelerated visualization

---

## Performance

### Benchmark Results

| Dataset | CGAL Time | Brute-Force Time | Speedup |
| --- | --- | --- | --- |
| 100k queries on 50k targets | 1.29s | 7.89s | **6.1x** |

Complexity:

* **CGAL:** `O(M log M) + O(N log M)`
* **Brute-force:** `O(N × M)`

### Real-World Performance

**Cholula, Mexico (5,097 nodes, 11,154 segments):**

* 1,000 assignments: 0.05 seconds
* GPU upload (one-time): 0.2 seconds
* Interactive rendering: 60 FPS

---

## Architecture

### Technology Stack

**Core:**

* **CGAL 6.0:** Computational geometry
* **pybind11:** Python-C++ bindings
* **NumPy/Pandas:** Data processing

**Geospatial:**

* **OSMnx:** OpenStreetMap data access
* **GeoPandas:** Spatial data handling
* **CGAL Spatial Indexing:** High-performance spatial queries
* **SciPy Spatial:** Fallback spatial indexing

**Visualization:**

* **VisPy:** GPU-accelerated rendering
* **PyQt5:** Window management

### Spatial Indexing Implementation

The library uses **CGAL spatial indexing** for all spatial operations:

* **CGAL Delaunay Triangulation:** High-performance spatial queries.
* **C++ Implementation:** Maximum performance for large datasets bypassing Python's GIL where possible.
* **Consistent Architecture:** Same infrastructure as `MapIndex`.
* **Real-time Capable:** Optimized for interactive applications.

**Usage:** All spatial operations automatically use CGAL under the hood:

```python
from acj.algorithms.graph import _find_node_clusters
# Uses CGAL Delaunay triangulation internally
clusters = _find_node_clusters(coords, threshold)

```

### Project Structure (Clean Architecture)

Our repository follows a modern Python layout, separating source code, tests, and C++ extensions:

```text
acj_project/
├── src/
│   └── acj/
│       ├── __init__.py          # Public API exports
│       ├── algorithms/          # Core logic & math
│       │   ├── graph.py         
│       │   └── map_index.py     
│       ├── data/                # Data manipulation
│       │   └── io.py            
│       ├── utils/               # Helpers & GUI
│       │   └── render.py        
│       └── core/                # C++ Source Code (Extension)
│           ├── CMakeLists.txt
│           └── src/
│               └── acj_core.cpp # CGAL pybind11 bindings
├── tests/
│   └── test_acj.py              # Pytest suite
├── examples/
│   ├── example_acj.py           
│   └── example_realtime.py      
├── build/                       # Compiled C++ binaries (Generated)
├── CMakeLists.txt               # Main CMake configuration
├── pyproject.toml               # Python package metadata
├── flake.nix                    # NixOS deterministic environment
├── Dockerfile                   # Cross-platform container
└── Makefile                     # Build automation shortcuts

```
---

## Development

### Running Tests

```bash
# Run all tests (if using local venv or Nix)
pytest tests/ -v

# Run a specific test class via Docker
docker run --user $(id -u):$(id -g) -v $(pwd):/workspace ubuntu-acj:1 \
  sh -c "PYTHONPATH=/workspace/build/src/acj/core:/workspace/src pytest tests/ -v -k TestMapIndex"

```

Test coverage:

* Graph data validation
* OSMnx integration
* CGAL spatial queries
* Assignment correctness
* Data preservation

---

### Build Workflow

```bash
# Rebuild Docker container after changes
make clean
make build

# Enter interactive debugging shell
make shell-user

# Inside container: compile C++ and test Python logic
cd build && cmake .. && make -j$(nproc)
export PYTHONPATH=/workspace/build/src/acj/core:/workspace/src

python3
>>> from acj.data.io import load_map
>>> graph = load_map("Liechtenstein")

```

---

## Known Limitations

1. **Segment assignment not implemented:** `assign_to_segments()` requires CGAL AABB tree implementation.
2. **Coordinates must be projected:** Latitude/longitude must be converted to a metric system (e.g., UTM) before assignment.

---

## Planned Features

* Point-to-segment assignment using AABB tree
* Export visualizations to image/video
* Temporal analysis support
* Network-based distance calculation
* Advanced graph simplification algorithms

---

## Troubleshooting

### Cannot connect to X server

Enable X11 forwarding for the visualization window:

```bash
xhost +local:docker
make example-realtime

```

### Module 'acj_core' not found

Python cannot find your compiled C++ extension. Ensure you have compiled it and exported the correct path:

```bash
mkdir -p build && cd build
cmake .. && make -j$(nproc)
export PYTHONPATH=$(pwd)/src/acj/core:$PYTHONPATH

```

### City name not found

* Verify your internet connection.
* Check the city name spelling (Nominatim format).
* Try a broader query (e.g., country name).

---

## License

MIT License

---

## Contributing

Issues and pull requests are welcome. Please include tests with new features to maintain our coverage standards.

**Acknowledgments:**

* **CGAL Project:** Computational geometry library
* **OSMnx:** OpenStreetMap data interface
* **VisPy:** GPU-accelerated visualization framework

