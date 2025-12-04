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
import numpy as np


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
    plt.ylabel("Execution Time (s)")
    plt.grid(True, axis="y", linestyle="--")
    plt.tight_layout()

    # Generate graphs in png
    plt.savefig(f"results/{graph_name.replace(' ', '_').lower()}.png")

    plt.show()

# same function for multiple csv files -> in one plot (maybe a better way to do ...)
def plot_thread_results_multiple(csv_paths: list[str], graph_names: list[str]):

    data_frames = [pd.read_csv(path) for path in csv_paths]

    threads = data_frames[0].columns[1:]
    x = np.arange(len(threads))

    num_groups = len(csv_paths)
    bar_width = 0.8 / num_groups  # total width = 0.8

    plt.figure(figsize=(10, 6))

    for i, (df, group_name) in enumerate(zip(data_frames, graph_names)):
        averages = df[threads].mean()
        stds = df[threads].std()

        # Shift each data set by index * bar_width
        positions = x + (i - num_groups/2) * bar_width + bar_width/2

        plt.bar(positions, averages, yerr=stds, width=bar_width, capsize=4, label=group_name)

    plt.xticks(x, threads) # set x-ticks to be in the center of grouped bars, because using numpy to have not nly x values and their y values. Whe had to shift them to create groups
    plt.title("Comparison of Execution Times")
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time (s)")
    plt.grid(True, axis="y", linestyle="--")
    plt.legend()
    plt.tight_layout()

    plt.savefig("results/comparison_execution_times.png")
    plt.show()




plot_thread_results("results/phil.csv", graph_name="Dining Philosophers Execution Time using Posix")
plot_thread_results("results/prod.csv", graph_name="Producer-consumer Execution Time using Posix")
plot_thread_results("results/read.csv", graph_name="Readers-writers Execution Time using Posix")

plot_thread_results_multiple(["results/spinlock_test1.csv", "results/spinlock_test2.csv"], ["test-and-set", "test-and-test-and-set"])

plot_thread_results("results/spinlock_phil.csv", graph_name="Dining Philosophers Execution Time")
plot_thread_results("results/spinlock_prod.csv", graph_name="Producer-consumer Execution Time")
plot_thread_results("results/spinlock_read.csv", graph_name="Readers-writers Execution Time")
