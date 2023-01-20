#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <unordered_map>
#include <iostream>

using namespace boost;

// Graph
typedef property<vertex_name_t, std::string> VertexProperty;
typedef adjacency_list<setS, vecS, bidirectionalS, VertexProperty> Graph;  // template params: edge-list, vertex list, "directedness", vertex properties
typedef property_map<Graph, vertex_name_t>::type URLMap;  // Map: vertex descriptor -> URL string

// Vertex
typedef graph_traits<Graph>::vertex_descriptor Vertex;

// Iterators
typedef graph_traits<Graph>::vertex_iterator VertIter;
typedef graph_traits<Graph>::edge_iterator EdgeIter;

const int MaxUrls = 20;
const std::string Domain = "highlands.edu";

void crawl_web(std::string start_url);
std::vector<std::string> &fetch_links(const std::string &url, std::vector<std::string> &urls);

#endif
