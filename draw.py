import matplotlib.pyplot as plt
import networkx as nx
import pandas as pd
import numpy as np


SEED = 2020
DATA_PATH = "data/"


def visualize_graph(graph_name):
    g = nx.Graph()
    coauthor_data = pd.read_csv(DATA_PATH + graph_name + ".txt")

    for _, row in coauthor_data.iterrows():
        g.add_edge(row['u'], row['v'])
    
    
    if graph_name == "SSB":
        # Give one of three colors
        node_colors = []
        for node in g.nodes():
            rgb = [0,0,0]
            rgb[node%3] = 1
            node_colors.append(tuple(rgb))

        # group by community
        pos = nx.circular_layout(g)
        k = 3
        angs = np.linspace(0, 2*np.pi, 1+k)
        cluster_locs = []
        rad = 3.5  # radius of circle
        for ea in angs:
            if ea > 0:
                cluster_locs.append(np.array([rad*np.cos(ea), rad*np.sin(ea)]))
        for node in pos.keys():
            color = node % 3
            frac = (node % 10)/10
            pos[node] = np.array([cluster_locs[color][0] + np.cos(frac*2*np.pi), cluster_locs[color][1] + np.sin(frac*2*np.pi)])
    else:
        pos = nx.spring_layout(g, seed=SEED)

    node_colors = []
    for node in g.nodes():
        rgb = [0,0,0]
        rgb[node%3] = 1
        node_colors.append(tuple(rgb))

    nx.draw(g, pos=pos, node_color=node_colors)
    plt.savefig("plots/GROUPED_" + graph_name + ".png")


if __name__ == "__main__":
    # plt.title("G(n=35, p=0.25)")
    # visualize_graph("GNP")
    # plt.clf()

    # plt.title("Caltech.edu Web (n=100)")
    # visualize_graph("web100")
    # plt.clf()

    # plt.title("Caltech.edu Web (n=300)")
    # visualize_graph("web300")
    # plt.clf()

    plt.title("SSBM(n=30, k=3, A=0.7, B=0.1)")
    visualize_graph("SSB")
    plt.clf()
