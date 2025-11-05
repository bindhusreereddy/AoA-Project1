# plot_greedy_runtime.py
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("greedy_timings.csv")

# average time for each n
means = df.groupby("n_boxes")["time_ms"].mean().reset_index()

plt.figure(figsize=(7,4))
plt.plot(means["n_boxes"], means["time_ms"], marker="o", label="Measured time")
plt.plot(means["n_boxes"], means["n_boxes"] *
         (means["time_ms"].iloc[0] / means["n_boxes"].iloc[0] *
          (1 + 0.3*np.log2(means["n_boxes"]/means["n_boxes"].iloc[0]))),
         linestyle="--", color="gray", label="~ O(n log n) trend")
plt.xlabel("Number of boxes (n)  [m = n]")
plt.ylabel("Runtime (milliseconds)")
plt.title("Tunnel Packing (Greedy) — Runtime vs n")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("greedy_runtime.png", dpi=160)
plt.show()
