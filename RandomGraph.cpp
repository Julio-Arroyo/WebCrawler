#include "RandomGraph.hpp"

void GNP(UndirectedGraph &g, int n, double p)
{
    for (int i = 0; i < n; i++) {
        std::cout << add_vertex(g) << std::endl;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            double v = ((double) rand() / RAND_MAX);
            if (v <= p) {
                add_edge(i, j, g);
            }
        }
    }
}

void SSB(UndirectedGraph &g, int n_vert, int n_communities, double a, double b)
{
    std::unordered_map<int, int> comm_label;
    for (int v = 0; v < n_vert; v++) {
        add_vertex(g);
        comm_label[v] = v % n_communities;
    }

    for (int u = 0; u < n_vert; u++) {
        for (int v = 0; v < n_vert; v++) {
            double sample = ((double) rand() / RAND_MAX);
            if ((comm_label[u] == comm_label[v] && sample <= a) ||
                    (comm_label[u] != comm_label[v] && sample <= b)) {
                add_edge(u, v, g);
            }
        }
    }
}

int main()
{
    UndirectedGraph g(0);
    GNP(g, 10, 0.1);
}
