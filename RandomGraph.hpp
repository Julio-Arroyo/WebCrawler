#ifndef RANDOMGRAPH_HPP
#define RANDOMGRAPH_HPP

#include "Crawler.hpp"

void SSB(UndirectedGraph &g, int n_vert, int n_communities, double a, double b);

void GNP(UndirectedGraph &g, int n, double p);

void save_graph(UndirectedGraph &g, std::string &fname);

#endif
