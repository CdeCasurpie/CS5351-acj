#include "graph_simplify.hpp"

// ================= Helpers Internos =================

std::set<std::pair<long, long>> trace_new_segments(
    const std::set<long>& intersection_ids,
    const AdjacencyMap& adjacency,
    const std::map<long, long>& node_to_new_node_map
) {
    std::set<std::pair<long, long>> new_segments_set;
    std::set<std::pair<long, long>> visited_edges; 

    for (long start_intersection_id : intersection_ids) {
        if (node_to_new_node_map.find(start_intersection_id) == node_to_new_node_map.end()) continue;
        long start_new_node_id = node_to_new_node_map.at(start_intersection_id);

        std::deque<long> q;
        std::set<long> visited_nodes_in_path;
        q.push_back(start_intersection_id);
        visited_nodes_in_path.insert(start_intersection_id);

        while (!q.empty()) {
            long current_node_id = q.front();
            q.pop_front();
            if (!adjacency.count(current_node_id)) continue;

            for (long neighbor_id : adjacency.at(current_node_id)) {
                if (visited_nodes_in_path.count(neighbor_id)) continue;
                std::pair<long, long> edge_key = std::minmax(current_node_id, neighbor_id);
                if (visited_edges.count(edge_key)) continue;

                if (intersection_ids.count(neighbor_id)) {
                    if (node_to_new_node_map.find(neighbor_id) == node_to_new_node_map.end()) continue;
                    long end_new_node_id = node_to_new_node_map.at(neighbor_id);

                    if (start_new_node_id != end_new_node_id) {
                        new_segments_set.insert(std::minmax(start_new_node_id, end_new_node_id));
                    }
                    visited_edges.insert(edge_key);
                } else {
                    visited_nodes_in_path.insert(neighbor_id);
                    q.push_back(neighbor_id);
                    visited_edges.insert(edge_key);
                }
            }
        }
    }
    return new_segments_set;
}

void build_graph_structures(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    NodeCoordMap& node_map,
    NodeDegreeMap& node_degrees,
    AdjacencyMap& adjacency
) {
    auto nodes_buf = nodes.request();
    auto segments_buf = segments.request();
    const double* nodes_ptr = static_cast<const double*>(nodes_buf.ptr);
    const double* segments_ptr = static_cast<const double*>(segments_buf.ptr);
    size_t n_nodes = static_cast<size_t>(nodes_buf.shape[0]);
    size_t n_segments = static_cast<size_t>(segments_buf.shape[0]);

    for (size_t i = 0; i < n_nodes; i++) {
        long node_id = static_cast<long>(nodes_ptr[i * 3 + 0]);
        double x = nodes_ptr[i * 3 + 1];
        double y = nodes_ptr[i * 3 + 2];
        node_map[node_id] = Point_pt(x, y);
        node_degrees[node_id] = 0;
    }
    for (size_t j = 0; j < n_segments; j++) {
        long start_id = static_cast<long>(segments_ptr[j * 3 + 1]);
        long end_id = static_cast<long>(segments_ptr[j * 3 + 2]);
        
        if (node_map.find(start_id) == node_map.end() || node_map.find(end_id) == node_map.end()) {
            continue; 
        }

        adjacency[start_id].push_back(end_id);
        adjacency[end_id].push_back(start_id);
        node_degrees[start_id]++;
        node_degrees[end_id]++;
    }
}

void build_graph_structures(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    NodeCoordMap& node_map,
    NodeDegreeMap& node_degrees,
    AdjacencyMap& adjacency,
    std::vector<SegmentInfo>& segments_info_list 
) {
    build_graph_structures(nodes, segments, node_map, node_degrees, adjacency);
    
    auto segments_buf = segments.request();
    const double* segments_ptr = static_cast<const double*>(segments_buf.ptr);
    size_t n_segments = static_cast<size_t>(segments_buf.shape[0]);

    for (size_t j = 0; j < n_segments; j++) {
        long seg_id = static_cast<long>(segments_ptr[j * 3 + 0]);
        long start_id = static_cast<long>(segments_ptr[j * 3 + 1]);
        long end_id = static_cast<long>(segments_ptr[j * 3 + 2]);

        if (node_map.find(start_id) == node_map.end() || node_map.find(end_id) == node_map.end()) {
            continue;
        }

        Point_pt p1 = node_map.at(start_id);
        Point_pt p2 = node_map.at(end_id);
        Segment_k seg_geom(p1, p2);

        segments_info_list.emplace_back(seg_id, start_id, end_id, p1, p2, seg_geom);
    }
}

// ================= Algoritmos Principales =================

py::tuple simplify_graph_topological_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments
) {
    NodeCoordMap node_map;
    NodeDegreeMap node_degrees;
    AdjacencyMap adjacency;
    build_graph_structures(nodes, segments, node_map, node_degrees, adjacency);

    std::set<long> intersection_ids;
    std::map<long, long> node_to_new_node_map;
    std::vector<std::tuple<long, double, double>> new_nodes_list;

    for (const auto& pair : node_degrees) {
        if (pair.second != 2) {
            long node_id = pair.first;
            intersection_ids.insert(node_id);
            node_to_new_node_map[node_id] = node_id;
            Point_pt p = node_map[node_id];
            new_nodes_list.emplace_back(node_id, p.x(), p.y());
        }
    }

    std::set<std::pair<long, long>> new_segments_set =
        trace_new_segments(intersection_ids, adjacency, node_to_new_node_map);

    std::vector<std::tuple<long, long, long, double, double, double, double>> new_segments_list;
    long new_segment_id = 0;
    for (const auto& seg_pair : new_segments_set) {
        long id1 = seg_pair.first;
        long id2 = seg_pair.second;
        Point_pt p1 = node_map[id1];
        Point_pt p2 = node_map[id2];
        new_segments_list.emplace_back(new_segment_id++, id1, id2, p1.x(), p1.y(), p2.x(), p2.y());
    }
    return py::make_tuple(py::cast(new_nodes_list), py::cast(new_segments_list));
}

py::tuple simplify_graph_minkowski_cgal(
	py::array_t<double> nodes,
	py::array_t<double> segments,
	double radius
){
	NodeCoordMap node_map;
	NodeDegreeMap node_degrees;
	AdjacencyMap adjacency;
	std::vector<SegmentInfo> segments_info_list;
	build_graph_structures(nodes, segments, node_map, node_degrees, adjacency, segments_info_list);
	Polygon_set_exact poly_set;
	for (const auto& seg_info : segments_info_list) {
        Point_pt p1 = std::get<3>(seg_info);
        Point_pt p2 = std::get<4>(seg_info);

        double dx = CGAL::to_double(p2.x() - p1.x());
        double dy = CGAL::to_double(p2.y() - p1.y());
        double len = std::sqrt(dx*dx + dy*dy);
        if (len == 0) continue;

        double nx = -dy / len * radius;
        double ny = dx / len * radius;

        Polygon_exact poly;
        poly.push_back(Exact_K::Point_2(CGAL::to_double(p1.x()) + nx, CGAL::to_double(p1.y()) + ny));
        poly.push_back(Exact_K::Point_2(CGAL::to_double(p2.x()) + nx, CGAL::to_double(p2.y()) + ny));
        poly.push_back(Exact_K::Point_2(CGAL::to_double(p2.x()) - nx, CGAL::to_double(p2.y()) - ny));
        poly.push_back(Exact_K::Point_2(CGAL::to_double(p1.x()) - nx, CGAL::to_double(p1.y()) - ny));

        if (poly.is_simple()) {
            if (poly.is_clockwise_oriented()) poly.reverse_orientation();
            poly_set.join(poly);
        }
    }

	std::vector<Polygon_with_holes_exact> res_polygons;
    poly_set.polygons_with_holes(std::back_inserter(res_polygons));
	std::vector<std::tuple<long, double, double>> new_nodes_list;
    std::vector<std::tuple<long, long, long, double, double, double, double>> new_segments_list;

    long node_id_counter = 0;
    long segment_id_counter = 0;
    std::map<Exact_K::Point_2, long> exact_node_map;

	for (const auto& pwh : res_polygons) {
        Exact_K exact_kernel;
		Straight_skeleton_ptr ss = CGAL::create_interior_straight_skeleton_2(
            pwh.outer_boundary().vertices_begin(),
            pwh.outer_boundary().vertices_end(),
            pwh.holes_begin(),
            pwh.holes_end(),
			exact_kernel
        );
        if (!ss) continue;

        for (auto edge = ss->halfedges_begin(); edge != ss->halfedges_end(); ++edge)
		{
            if (!edge->is_bisector() || !edge->is_inner_bisector()) continue; 
            
            auto v1 = edge->vertex();
            auto v2 = edge->opposite()->vertex();
            
            // Solo nos quedamos con el eje central puro, descartando las diagonales a las esquinas
            if (!v1->is_skeleton() || !v2->is_skeleton()) continue;

            Exact_K::Point_2 p1 = v1->point();
            Exact_K::Point_2 p2 = v2->point();

            long id1, id2;
            if (exact_node_map.find(p1) == exact_node_map.end()) {
                id1 = node_id_counter++;
                exact_node_map[p1] = id1;
                new_nodes_list.emplace_back(id1, CGAL::to_double(p1.x()), CGAL::to_double(p1.y()));
            } else id1 = exact_node_map[p1];

            if (exact_node_map.find(p2) == exact_node_map.end()) {
                id2 = node_id_counter++;
                exact_node_map[p2] = id2;
                new_nodes_list.emplace_back(id2, CGAL::to_double(p2.x()), CGAL::to_double(p2.y()));
            } else id2 = exact_node_map[p2];

            // Usamos NUESTROS ids generados para evitar aristas duplicadas
            if (id1 < id2) {
                new_segments_list.emplace_back(
                    segment_id_counter++, id1, id2,
                    CGAL::to_double(p1.x()), CGAL::to_double(p1.y()),
                    CGAL::to_double(p2.x()), CGAL::to_double(p2.y())
                );
				
        	}   
		}
    }
    
    return py::make_tuple(py::cast(new_nodes_list), py::cast(new_segments_list));
}

py::tuple simplify_graph_geometric_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double threshold
) {
    NodeCoordMap node_map;
    NodeDegreeMap node_degrees;
    AdjacencyMap adjacency;
    build_graph_structures(nodes, segments, node_map, node_degrees, adjacency);

    std::set<long> intersection_ids;
    std::vector<long> intersection_id_vec;
    std::vector<Point_pt> intersection_points;

    for (const auto& pair : node_degrees) {
        if (pair.second != 2) {
            long node_id = pair.first;
            intersection_ids.insert(node_id);
            intersection_id_vec.push_back(node_id);
            intersection_points.push_back(node_map[node_id]);
        }
    }
    if (intersection_ids.empty()) {
        return simplify_graph_topological_cgal(nodes, segments);
    }

    DT dt;
    std::map<Point_pt, int> point_to_idx_map;
    for (size_t i = 0; i < intersection_points.size(); i++) {
        dt.insert(intersection_points[i]);
        point_to_idx_map[intersection_points[i]] = static_cast<int>(i);
    }

    std::vector<int> parent(intersection_points.size());
    for (size_t i = 0; i < parent.size(); i++) parent[i] = static_cast<int>(i);

    std::function<int(int)> find;
    find = [&parent, &find](int x) -> int {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };
    auto union_sets = [&find, &parent](int x, int y) {
        int px = find(x); int py = find(y);
        if (px != py) parent[px] = py;
    };

    double threshold_sq = threshold * threshold;
    for (auto it = dt.finite_edges_begin(); it != dt.finite_edges_end(); ++it) {
        auto v1 = it->first->vertex((it->second + 1) % 3);
        auto v2 = it->first->vertex((it->second + 2) % 3);
        if (dt.is_infinite(v1) || dt.is_infinite(v2)) continue;
        if (CGAL::squared_distance(v1->point(), v2->point()) <= threshold_sq) {
            union_sets(point_to_idx_map[v1->point()], point_to_idx_map[v2->point()]);
        }
    }

    std::map<int, std::vector<int>> clusters;
    for (size_t i = 0; i < parent.size(); i++) {
        clusters[find(static_cast<int>(i))].push_back(static_cast<int>(i));
    }

    std::vector<std::tuple<long, double, double>> new_nodes_list;
    std::map<long, long> node_to_new_node_map;
    std::map<long, Point_pt> new_node_coords;
    long new_cluster_id = 0;

    for (const auto& cluster : clusters) {
        double total_x = 0, total_y = 0;
        int count = 0;
        for (int idx : cluster.second) {
            long original_node_id = intersection_id_vec[idx];
            node_to_new_node_map[original_node_id] = new_cluster_id;
            Point_pt p = intersection_points[idx];
            total_x += p.x();
            total_y += p.y();
            count++;
        }
        double centroid_x = total_x / count;
        double centroid_y = total_y / count;
        new_nodes_list.emplace_back(new_cluster_id, centroid_x, centroid_y);
        new_node_coords[new_cluster_id] = Point_pt(centroid_x, centroid_y);
        new_cluster_id++;
    }

    std::set<std::pair<long, long>> new_segments_set =
        trace_new_segments(intersection_ids, adjacency, node_to_new_node_map);

    std::vector<std::tuple<long, long, long, double, double, double, double>> new_segments_list;
    long new_segment_id = 0;
    for (const auto& seg_pair : new_segments_set) {
        long id1 = seg_pair.first;
        long id2 = seg_pair.second;
        Point_pt p1 = new_node_coords[id1];
        Point_pt p2 = new_node_coords[id2];
        new_segments_list.emplace_back(new_segment_id++, id1, id2, p1.x(), p1.y(), p2.x(), p2.y());
    }
    return py::make_tuple(py::cast(new_nodes_list), py::cast(new_segments_list));
}

py::tuple simplify_graph_parallel_cgal(
    py::array_t<double> nodes,
    py::array_t<double> segments,
    double distance_threshold,
    double angle_threshold_deg
) {
    NodeCoordMap node_map;
    NodeDegreeMap node_degrees;
    AdjacencyMap adjacency;
    std::vector<SegmentInfo> segments_info_list;

    build_graph_structures(nodes, segments, node_map, node_degrees, adjacency, segments_info_list);

    if (segments_info_list.empty()) {
        return py::make_tuple(py::cast(std::vector<std::tuple<long, double, double>>{}), py::cast(std::vector<std::tuple<long, long, long, double, double, double, double>>{}));
    }

    std::vector<int> parent(segments_info_list.size());
    for (size_t i = 0; i < parent.size(); i++) parent[i] = static_cast<int>(i);

    std::function<int(int)> find;
    find = [&parent, &find](int x) -> int {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };
    auto union_sets = [&find, &parent](int x, int y) {
        int px = find(x); int py = find(y);
        if (px != py) parent[px] = py;
    };

    double threshold_sq = distance_threshold * distance_threshold;
    double angle_cos_threshold = std::cos(angle_threshold_deg * acos(-1.0) / 180.0);

    for (size_t i = 0; i < segments_info_list.size(); ++i) {
        const auto& seg_i_info = segments_info_list[i];
        const Segment_k& seg_i = std::get<5>(seg_i_info);
        Vector_k vec_i = seg_i.to_vector();

        for (size_t j = i + 1; j < segments_info_list.size(); ++j) {
            const auto& seg_j_info = segments_info_list[j];
            const Segment_k& seg_j = std::get<5>(seg_j_info);

            double dist_sq = CGAL::to_double(CGAL::squared_distance(seg_i, seg_j));
            if (dist_sq > threshold_sq) continue;

            Vector_k vec_j = seg_j.to_vector();
            double dot_product = CGAL::to_double(vec_i * vec_j);
            double magnitude_prod = CGAL::to_double(vec_i.squared_length() * vec_j.squared_length());
            
            if (magnitude_prod == 0) continue;
            double cos_angle = dot_product / std::sqrt(magnitude_prod);

            if (std::abs(cos_angle) > angle_cos_threshold) {
                union_sets(static_cast<int>(i), static_cast<int>(j));
            }
        }
    }
    
    std::map<int, std::vector<int>> segment_clusters;
    for (size_t i = 0; i < parent.size(); i++) {
        segment_clusters[find(static_cast<int>(i))].push_back(static_cast<int>(i));
    }

    std::vector<std::tuple<long, double, double>> new_nodes_list;
    std::map<long, long> node_to_new_node_map;
    std::map<long, Point_pt> new_node_coords;
    long new_cluster_id_counter = 0;
    long new_node_id_offset = 10000000;

    for (const auto& cluster_pair : segment_clusters) {
        const auto& segment_indices = cluster_pair.second;
        if (segment_indices.size() == 1) continue; 

        std::set<long> original_node_ids;
        for (int idx : segment_indices) {
            original_node_ids.insert(std::get<1>(segments_info_list[idx])); // Start ID
            original_node_ids.insert(std::get<2>(segments_info_list[idx])); // End ID
        }

        long new_id = new_node_id_offset + (new_cluster_id_counter++);
        double total_x = 0, total_y = 0;
        for (long original_id : original_node_ids) {
            Point_pt p = node_map.at(original_id);
            total_x += CGAL::to_double(p.x());
            total_y += CGAL::to_double(p.y());
        }
        double centroid_x = total_x / original_node_ids.size();
        double centroid_y = total_y / original_node_ids.size();
        Point_pt centroid_p(centroid_x, centroid_y);

        for (long original_id : original_node_ids) {
            node_to_new_node_map[original_id] = new_id;
        }
        new_nodes_list.emplace_back(new_id, centroid_x, centroid_y);
        new_node_coords[new_id] = centroid_p;
    }

    for (const auto& pair : node_degrees) {
        long node_id = pair.first;
        if (pair.second != 2 && node_to_new_node_map.find(node_id) == node_to_new_node_map.end()) {
            node_to_new_node_map[node_id] = node_id;
            Point_pt p = node_map.at(node_id);
            new_nodes_list.emplace_back(node_id, CGAL::to_double(p.x()), CGAL::to_double(p.y()));
            new_node_coords[node_id] = p;
        }
    }

    std::set<long> final_intersections;
    for(const auto& pair : node_to_new_node_map) {
        final_intersections.insert(pair.first);
    }

    std::set<std::pair<long, long>> new_segments_set =
        trace_new_segments(final_intersections, adjacency, node_to_new_node_map);

    std::vector<std::tuple<long, long, long, double, double, double, double>> final_segments_list;
    long final_segment_id = 0;

    for (const auto& seg_pair : new_segments_set) {
        long id1 = seg_pair.first;
        long id2 = seg_pair.second;
        if (new_node_coords.find(id1) == new_node_coords.end() || new_node_coords.find(id2) == new_node_coords.end()) {
            continue;
        }
        Point_pt p1 = new_node_coords.at(id1);
        Point_pt p2 = new_node_coords.at(id2);
        final_segments_list.emplace_back(
            final_segment_id++,
            id1, id2,
            CGAL::to_double(p1.x()), CGAL::to_double(p1.y()),
            CGAL::to_double(p2.x()), CGAL::to_double(p2.y())
        );
    }
    return py::make_tuple(py::cast(new_nodes_list), py::cast(final_segments_list));
}
