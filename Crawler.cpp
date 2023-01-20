#include "Crawler.hpp"

std::vector<std::string> &fetch_links(std::string &url)
{
    std::vector<std::string> urls;
    // TODO: system call to fetcher3.py
    return urls;
}

void crawl_web(std::string start_url)
{
    // Initialize data structures
    Graph webgraph(0);
    URLMap urls = get(vertex_name, webgraph);
    std::unordered_map<std::string, Vertex> url_to_vert;
    std::vector<Vertex> stack;

    stack.push_back(add_vertex(webgraph));
    while (stack.size() > 0 && url_to_vert.size() < MaxUrls)
    {
        Vertex curr_v = stack.back();
        stack.pop_back();
        std::string &curr_url = urls[curr_v];

        std::vector<std::string> fetched_urls = fetch_links(curr_url);
        for (int i = 0; i < fetched_urls.size(); i++)
        {
            Vertex next_v;
            if (url_to_vert.count(fetched_urls[i]) == 0)
            {
                next_v = add_vertex(webgraph);
                put(urls, next_v, fetched_urls[i]);
                stack.push_back(next_v);
            }
            else
            {
                next_v = url_to_vert[fetched_urls[i]];
            }
            add_edge(curr_v, next_v, webgraph);
        }
    }
}

int main()
{
    // crawl_web("MyURL");
    const char *cmd = "python3 fetcher3.py https://www.highlands.edu.sv";
    FILE *f = popen(cmd, (const char *) "r");
    char buff[255];
    fgets(buff, 255, f);
    std::cout << buff << std::endl;
    return 0;
}
