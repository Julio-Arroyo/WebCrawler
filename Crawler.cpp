#include "Crawler.hpp"

std::vector<std::string> &fetch_links(const std::string &url, std::vector<std::string> &urls)
{
    std::string cmd = "python3 fetcher3.py " + url;
    FILE *fp = popen(cmd.c_str(), (const char *) "r");
    if (fp == NULL)
    {
        throw std::system_error(EDOM, std::generic_category(), "hello world");
    }

    std::string curr_url;
    while (true)
    {
        char c = fgetc(fp);
        if (feof(fp))
        {
            if (curr_url.find(Domain) != -1)
            {
                urls.push_back(curr_url);
            }
            curr_url.clear();
            break;
        } else if (c == ',' || c == '\n')
        {
            if (curr_url.find(Domain) != -1)
            {
                urls.push_back(curr_url);
            }
            curr_url.clear();
        } else
        {
            curr_url += c;
        }
    }
    return urls;
}

void crawl_web(std::string start_url,
               Graph &webgraph,
               URLMap &urls,
               std::unordered_map<std::string, Vertex> &url_to_vert)
{
    std::vector<Vertex> stack;

    Vertex start_vertex = add_vertex(webgraph);
    put(urls, start_vertex, start_url);
    url_to_vert[start_url] = start_vertex;
    stack.push_back(start_vertex);
    while (stack.size() > 0 && url_to_vert.size() < MaxUrls)
    {
        Vertex curr_v = stack.back();
        stack.pop_back();
        std::string &curr_url = urls[curr_v];

        std::vector<std::string> fetched_urls;
        fetch_links(curr_url, fetched_urls);
        for (int i = 0; i < fetched_urls.size(); i++)
        {
            Vertex next_v;
            if (url_to_vert.count(fetched_urls[i]) == 0)
            {
                next_v = add_vertex(webgraph);
                put(urls, next_v, fetched_urls[i]);
                url_to_vert[fetched_urls[i]] = next_v;
                stack.push_back(next_v);
            }
            else
            {
                next_v = url_to_vert[fetched_urls[i]];
            }
            add_edge(curr_v, next_v, webgraph);
        }
    }

    // DISPLAY
    VertIter vi, vi_end;
    std::cout << "URLs: " << std::endl;
    for (tie(vi, vi_end) = vertices(webgraph); vi != vi_end; vi++)
    {
        Vertex curr_ver = *vi;
        std::cout << "$%$ " << urls[curr_ver] << std::endl;
    }
    std::cout << std::endl;

    EdgeIter ei, ei_end;
    std::cout << "EDGES: ";
    for (tie(ei, ei_end) = edges(webgraph); ei != ei_end; ei++)
    {
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

int main()
{
    Graph webgraph(0);
    URLMap urls = get(vertex_name, webgraph);  // PropertyMap to get URLs from vertex descriptors
    std::unordered_map<std::string, Vertex> url_to_vert;

    crawl_web("https://www.highlands.edu.sv/", webgraph, urls, url_to_vert);

    MapPtr in_deg_dist, out_deg_dist;
    tie(in_deg_dist, out_deg_dist) = get_degree_dist(webgraph);

    // Record degree distributions
    std::ofstream in_deg_f, out_deg_f;
    in_deg_f.open("in_deg_dist.txt");
    for (const std::pair<int, int> entry : (*in_deg_dist))
    {
        in_deg_f << entry.first << "," << entry.second << std::endl;
    }
    in_deg_f.close();

    out_deg_f.open("out_deg_dist.txt");
    for (const std::pair<int, int> entry : (*out_deg_dist))
    {
        out_deg_f << entry.first << "," << entry.second << std::endl;
    }
    out_deg_f.close();
    
    return 0;
}
