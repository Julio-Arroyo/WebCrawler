#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <iostream>
#include "Crawler.hpp"

using namespace boost;

int main()
{
    typedef property<vertex_name_t, std::string> VertexProperty;
    // Graph: template params: edge-list, vertex list, "directedness", vertex properties
    typedef adjacency_list<setS, vecS, bidirectionalS, VertexProperty> Graph;
    typedef property_map<Graph, vertex_name_t>::type URLMap;  // Map: vertex descriptor -> URL string

    // Vertices and edges
    typedef graph_traits<Graph>::vertex_descriptor Vertex;

    // Iterators
    typedef graph_traits<Graph>::vertex_iterator VertIter;
    typedef graph_traits<Graph>::edge_iterator EdgeIter;

    Graph webgraph(0);
    URLMap urls = get(vertex_name, webgraph);
    std::cout << "Initial size: " << num_vertices(webgraph) << std::endl;

    Vertex u = add_vertex(webgraph);
    put(urls, u, "A");
    Vertex v = add_vertex(webgraph);
    put(urls, v, "B");
    Vertex w = add_vertex(webgraph);
    put(urls, w, "C");
    Vertex x = add_vertex(webgraph);
    put(urls, x, "D");

    add_edge(u, v, webgraph);
    add_edge(v, w, webgraph);
    add_edge(v, x, webgraph);

    std::cout << "New # vertices: " << num_vertices(webgraph) << std::endl;
    std::cout << "Num edges: " << num_edges(webgraph) << std::endl;

    VertIter vi, vi_end;
    std::cout << "URL set: ";
    for (tie(vi, vi_end) = vertices(webgraph); vi != vi_end; vi++)
    {
        Vertex curr_ver = *vi;
        std::cout << urls[curr_ver] << ", ";
    }
    std::cout << std::endl;

    EdgeIter ei, ei_end;
    std::cout << "Edge Set: ";
    for (tie(ei, ei_end) = edges(webgraph); ei != ei_end; ei++)
    {
        std::cout << "(" << urls[source(*ei, webgraph)] << "," << urls[target(*ei, webgraph)] << "), ";
    }
    std::cout << std::endl;

    return 0;
}
