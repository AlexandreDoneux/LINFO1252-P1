# Graph generation script
#
# Uses csv files to generate graphs for the different experiments
# - philosophers.csv
# - producers_consumers.csv
# - writers_readers.csv
# - atomic_exclusion_algo.csv ("test-and-set", "test-and-test-and-set")

# Those graphs represent the computation or access speed for different number of threads. Each one contains mean values ad standart deviation.
# mean = sum off all values divided by the number of values
# standart deviation = see formula online, warning : not the one for population but for sample


import matplotlib.pyplot as plt
import pandas as pd


def plot_thread_results(csv_path, graph_name):
    data_frame = pd.read_csv(csv_path)
    threads = data_frame.columns[1:]

    averages = data_frame[threads].mean()
    standard_deviations = data_frame[threads].std()

    # Plot
    plt.figure(figsize=(10, 6))
    plt.bar(threads, averages, yerr=standard_deviations, ecolor="red", capsize=5)

    plt.title(graph_name)
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time")
    plt.grid(True)
    plt.tight_layout()

    # Generate graphs in png
    plt.savefig(f"results/{graph_name.replace(' ', '_').lower()}.png")

    plt.show()




plot_thread_results("results/phil.csv", graph_name="Dining Philosophers Execution Time")
plot_thread_results("results/prod.csv", graph_name="Producer-consumer Execution Time")
plot_thread_results("results/read.csv", graph_name="Readers-writers Execution Time")



# adding saving as png
