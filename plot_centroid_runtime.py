# plot_centroid_runtime.py
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("centroid_timings.csv")
means = df.groupby("n_nodes")["time_ms"].mean().reset_index()

plt.figure(figsize=(7,4))
plt.plot(means["n_nodes"], means["time_ms"], marker="o", label="Measured time")
plt.plot(means["n_nodes"],
         means["n_nodes"]*np.log2(means["n_nodes"])/5000,
         "--", color="gray", label="~O(n log n) trend")
plt.xlabel("Number of tree nodes (n)")
plt.ylabel("Runtime (milliseconds)")
plt.title("Centroid Decomposition Runtime Verification")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("centroid_runtime.png", dpi=160)
plt.show()