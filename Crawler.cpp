#include "Crawler.hpp"

std::vector<std::string> &fetch_links(const std::string &url, std::vector<std::string> &urls)
{
    std::string cmd = "python3 fetcher3.py " + url;
    FILE *fp = popen(cmd.c_str(), (const char *) "r");
    if (fp == NULL) { throw std::system_error(EDOM, std::generic_category(), "hello world"); }

    std::string curr_url;
    while (true) {
        char c = fgetc(fp);
        if (feof(fp)) {
            if (curr_url.find(Domain) != -1 && curr_url.find("calendar") == -1) {
                urls.push_back(curr_url);
            }
            curr_url.clear();
            break;
        } else if (c == ',' || c == '\n') {
            if (curr_url.find(Domain) != -1 && curr_url.find("calendar") == -1) {
                urls.push_back(curr_url);
            }
            curr_url.clear();
        } else {
            curr_url += c;
        }
    }
    return urls;
}

Vertex create_vertex(std::string &url,
                     Graph &webgraph,
                     URLMap &urls,
                     std::unordered_map<std::string, Vertex> &url_to_vert)
{
    Vertex u = add_vertex(webgraph);
    put(urls, u, url);
    url_to_vert[url] = u;
    return u;
}

void crawl_web(std::string start_url,
               Graph &webgraph,
               URLMap &urls,
               std::unordered_map<std::string, Vertex> &url_to_vert)
{
    std::vector<std::string> stack;
    Vertex start_v = create_vertex(start_url, webgraph, urls, url_to_vert);
    stack.push_back(start_url);

    while (stack.size() > 0) {
        std::string curr_url = stack.back();
        stack.pop_back();
        assert(url_to_vert.count(curr_url) == 1);
        Vertex u = url_to_vert[curr_url];

        // Create edge with every neighbor
        std::vector<std::string> fetched_urls;
        fetch_links(curr_url, fetched_urls);
        for (int i = 0; i < fetched_urls.size(); i++) {
            Vertex v;
            if (url_to_vert.count(fetched_urls[i]) == 0) {  // New (unvisited) URL
                if (url_to_vert.size() > MaxUrls) {
                    continue;  // Stop expanding BFS if already visited enough sites
                }
                v = create_vertex(fetched_urls[i], webgraph, urls, url_to_vert);
                stack.push_back(fetched_urls[i]);
            } else {
                v = url_to_vert[fetched_urls[i]];
            }
            add_edge(u, v, webgraph);
        }
    }
}

void print_graph(Graph &webgraph, URLMap &urls)
{
    VertIter vi, vi_end;
    std::cout << "URLs: " << std::endl;
    for (tie(vi, vi_end) = vertices(webgraph); vi != vi_end; vi++) {
        Vertex curr_ver = *vi;
        std::cout << "$%$ " << urls[curr_ver] << std::endl;
    }
    std::cout << std::endl;

    EdgeIter ei, ei_end;
    std::cout << "EDGES: ";
    for (tie(ei, ei_end) = edges(webgraph); ei != ei_end; ei++) {
        std::cout << "$%$ (" << urls[source(*ei, webgraph)] << "," << urls[target(*ei, webgraph)] << ")" << std::endl;
    }
    std::cout << std::endl;
}

std::pair<MapPtr, MapPtr> get_degree_dist(Graph &webgraph)
{
    MapPtr in_degree_dist(new std::unordered_map<int, int>());
    MapPtr out_degree_dist(new std::unordered_map<int, int>());

    VertIter vi, vi_end;
    for (tie(vi, vi_end) = vertices(webgraph); vi != vi_end; vi++)
    {
        Vertex curr_vert = *vi;
        Degree n_incoming = in_degree(curr_vert, webgraph);
        Degree n_outgoing = out_degree(curr_vert, webgraph);

        if (in_degree_dist->count(n_incoming) == 0)
        {
            (*in_degree_dist)[n_incoming] = 1;
        } else
        {
            (*in_degree_dist)[n_incoming] += 1;
        }

        if (out_degree_dist->count(n_outgoing) == 0)
        {
            (*out_degree_dist)[n_outgoing] = 1;
        } else
        {
            (*out_degree_dist)[n_outgoing] += 1;
        }
    }
    std::pair<MapPtr, MapPtr> degree_dist(in_degree_dist, out_degree_dist);
    return degree_dist;
}

void copy_dir_to_undir(Graph &dir_graph, UndirectedGraph &undir_graph)
{
    EdgeIter ei, ei_end;
    for (tie(ei, ei_end) = edges(dir_graph); ei != ei_end; ei++) {
        Vertex u = source(*ei, dir_graph);
        Vertex v = target(*ei, dir_graph);
        add_edge(u, v, undir_graph);
    }  
}

void save_degree_dist(MapPtr &in_deg_dist, MapPtr &out_deg_dist)
{
    std::ofstream in_deg_f, out_deg_f;
    in_deg_f.open("data/in_deg_dist.txt");
    in_deg_f << "deg,freq" << std::endl;
    for (const std::pair<int, int> entry : (*in_deg_dist)) {
        in_deg_f << entry.first << "," << entry.second << std::endl;
    }
    in_deg_f.close();

    out_deg_f.open("data/out_deg_dist.txt");
    out_deg_f << "deg,freq" << std::endl;
    for (const std::pair<int, int> entry : (*out_deg_dist)) {
        out_deg_f << entry.first << "," << entry.second << std::endl;
    }
    out_deg_f.close();
}

void compute_clustering_coeffs(UndirectedGraph &g)
{
    ClusteringContainer coefs(num_vertices(g));
    ClusteringMap cm(coefs, g);
    float cc = all_clustering_coefficients(g, cm);
    std::cout << "ANSWER: Overall clustering coefficient: " << cc << std::endl;

    // Find average clustering coefficient
    float avg_ci = 0;
    VertIter i, end;
    for (tie(i, end) = vertices(g); i != end; ++i) {
        avg_ci += get(cm, *i);  // clustering coefficient for vertex 'i'
    }
    avg_ci /= num_vertices(g);
    std::cout << "ANSWER: Average clustering coefficient: " << avg_ci << std::endl;
}

void compute_diameter(UndirectedGraph &g)
{
    // Compute the distances between all pairs of vertices using
    // the Floyd-Warshall algorithm. Note that the weight map is
    // created so that every edge has a weight of 1.
    DistanceMatrix distances(num_vertices(g));
    DistanceMatrixMap dm(distances, g);
    WeightMap wm(1);
    floyd_warshall_all_pairs_shortest_paths(g, dm, weight_map(wm));

    // Compute the eccentricities for graph - this computation returns
    // both the radius and diameter as well.
    int r, d;
    EccentricityContainer eccs(num_vertices(g));
    EccentricityMap em(eccs, g);
    tie(r, d) = all_eccentricities(g, dm, em);

    // Print the closeness centrality of each vertex.
    VertIter vi, vi_end, vj, vj_end;
    float avg_dist = 0;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
        for (tie(vj, vj_end) = vertices(g); vj != vj_end; ++vj) {
            avg_dist += dm[*vi][*vj];
        }
    }
    avg_dist /= (2.0 * num_vertices(g));  // Note factor of two since I added each distance twice
    std::cout << "ANSWER: radius: " << r << std::endl;
    std::cout << "ANSWER: max diameter: " << d << std::endl;
    std::cout << "ANSWER: AVG diameter: " << avg_dist << std::endl;
}

int main()
{
    Graph webgraph(0);
    URLMap urls = get(vertex_name, webgraph);  // PropertyMap to get URLs from vertex descriptors
    std::unordered_map<std::string, Vertex> url_to_vert;

    crawl_web("https://www.caltech.edu/", webgraph, urls, url_to_vert);
    print_graph(webgraph);

    MapPtr in_deg_dist, out_deg_dist;
    tie(in_deg_dist, out_deg_dist) = get_degree_dist(webgraph);
    save_degree_dist(in_deg_dist, out_deg_dist);

    // To compute diameters and clustering coefficients, we need an undirected graph
    UndirectedGraph undir_webgraph(0);
    copy_dir_to_undir(webgraph, undir_webgraph);
    if (num_vertices(undir_webgraph) != num_vertices(webgraph)) {
        std::cout << "VERTEX COUNT DISAGREE: " << num_vertices(undir_webgraph) << "!=" << num_vertices(webgraph) << std::endl;
    }
    compute_clustering_coeffs(undir_webgraph);
    compute_diameter(undir_webgraph);

    return 0;
}
