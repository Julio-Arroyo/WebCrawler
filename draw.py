import matplotlib.pyplot as plt
import networkx as nx
import pandas as pd

SEED = 2020
DATA_PATH = "data/"


def visualize_graph(graph_name):
    g = nx.Graph()
    coauthor_data = pd.read_csv(DATA_PATH + graph_name + ".txt")

    for _, row in coauthor_data.iterrows():
        g.add_edge(row['u'], row['v'])
    
    pos = nx.spring_layout(g, seed=SEED)
    nx.draw(g, pos=pos)
    plt.savefig("plots/" + graph_name + ".png")


if __name__ == "__main__":
    plt.title("G(n=35, p=0.25)")
    visualize_graph("GNP")
    plt.clf()

    plt.title("Caltech.edu Web (n=100)")
    visualize_graph("web100")
    plt.clf()

    plt.title("Caltech.edu Web (n=300)")
    visualize_graph("web300")
    plt.clf()

    plt.title("SSBM(n=30, k=3, A=0.7, B=0.1)")
    visualize_graph("SSB")
    plt.clf()
