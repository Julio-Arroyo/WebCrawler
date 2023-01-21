#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <boost/graph/clustering_coefficient.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace boost;

// Graph
typedef property<vertex_name_t, std::string> VertexProperty;
typedef adjacency_list<setS, vecS, bidirectionalS, VertexProperty> Graph;  // template params: edge-list, vertex list, "directedness", vertex properties
typedef adjacency_list<setS, vecS, undirectedS> UndirectedGraph;
typedef property_map<Graph, vertex_name_t>::type URLMap;  // Map: vertex descriptor -> URL string
typedef graph_traits<Graph>::degree_size_type Degree;
// Vertex
typedef graph_traits<Graph>::vertex_descriptor Vertex;

// Iterators
typedef graph_traits<Graph>::vertex_iterator VertIter;
typedef graph_traits<Graph>::edge_iterator EdgeIter;

// Clustering
typedef exterior_vertex_property<UndirectedGraph, float >::container_type ClusteringContainer;
typedef exterior_vertex_property<UndirectedGraph, float >::map_type ClusteringMap;

typedef shared_ptr<std::unordered_map<int, int> > MapPtr;

const int MaxUrls = 2000;
const std::string Domain = "caltech.edu";


std::vector<std::string> &fetch_links(const std::string &url, std::vector<std::string> &urls);

Vertex create_vertex(std::string &url,
                     Graph &webgraph,
                     URLMap &urls,
                     std::unordered_map<std::string, Vertex> &url_to_vert);

void crawl_web(std::string start_url,
               Graph &webgraph,
               URLMap &urls,
               std::unordered_map<std::string, Vertex> &url_to_vert);

void print_graph(Graph &webgraph);

std::pair<MapPtr, MapPtr> get_degree_dist(Graph &webgraph);

void save_degree_dist(MapPtr &in_deg_dist, MapPtr &out_deg_dist);

void copy_dir_to_undir(Graph &dir_graph, UndirectedGraph &undir_graph);

void compute_clustering_coeffs(UndirectedGraph &g);

#endif
