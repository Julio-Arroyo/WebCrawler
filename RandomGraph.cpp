#include "RandomGraph.hpp"

void GNP(UndirectedGraph &g, int n, double p)
{
    for (int i = 0; i < n; i++) {
        add_vertex(g);
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

void save_graph(UndirectedGraph &g, std::string &fname)
{
    std::ofstream f;
    std::string fpath = "data/" + fname;
    f.open(fpath);

    graph_traits<UndirectedGraph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ei++) {
        f << source(*ei, g) << "," << target(*ei, g) << std::endl;
    }
    f.close();
}

int main()
{
    UndirectedGraph gnp(0);
    GNP(gnp, 35, 0.25);
    std::string fname_gnp = "GNP.txt";
    save_graph(gnp, fname_gnp);

    UndirectedGraph ssb(0);
    SSB(ssb, 30, 3, 0.7, 0.1);
    std::string fname_ssb = "SSB.txt";
    save_graph(ssb, fname_ssb);
}
