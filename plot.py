import matplotlib.pyplot as plt
import pandas as pd


IN_DEG_PATH = "data/in_deg_dist.txt"
OUT_DEG_PATH = "data/out_deg_dist.txt"


def plot_dist(deg_dist, deg_type):
    s_deg_dist = deg_dist.sort_values(by=['deg'])
    freqs = s_deg_dist['freq'].to_numpy()
    cdf = freqs.cumsum() / freqs.sum()
    ccdf = 1 - cdf

    plt.title("CCDF")
    plt.ylabel("Pr(X>x)")
    plt.xlabel(deg_type)
    plt.scatter(s_deg_dist['deg'], ccdf)
    plt.savefig(f"plots/CCDF_{deg_type}.png")
    plt.clf()

    plt.title("Hyperlinks per page")
    plt.ylabel("Frequency")
    plt.xlabel(deg_type)
    # degrees = []
    # for idx, row in s_deg_dist.iterrows():
    #     for _ in range(row['freq']):
    #         degrees.append(s_deg_dist['deg'])
    if deg_type == "In-Degree":
        plt.xlim([0,150])
    # else:
    #     plt.xlim([0,180])
    plt.bar(s_deg_dist['deg'], s_deg_dist['freq'])
    plt.savefig(f"plots/HIST_{deg_type}.png")
    plt.clf()


if __name__ == "__main__":
    in_deg_dist = pd.read_csv(IN_DEG_PATH)
    out_deg_dist = pd.read_csv(OUT_DEG_PATH)

    # plot_dist(in_deg_dist, "In-Degree")
    plot_dist(out_deg_dist, "Out-Degree")
