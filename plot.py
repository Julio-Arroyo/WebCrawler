import matplotlib.pyplot as plt
import pandas as pd


IN_DEG_PATH = "./in_deg_dist.txt"
OUT_DEG_PATH = "./out_deg_dist.txt"

if __name__ == "__main__":
    in_deg_dist = pd.read_csv(IN_DEG_PATH)
    out_deg_dist = pd.read_csv(OUT_DEG_PATH)

    in_deg_dist.plot.hist()
    out_deg_dist.plot.hist()

    plt.show()