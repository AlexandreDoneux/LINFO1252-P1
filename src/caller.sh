#!/bin/bash

set -x # for debugging -> remove later

# Function to run and measure execution time
# Usage: run_and_measure <executable> <csv_file> "<threads_array>" <num_runs>
run_and_measure() {
    local executable="$1" # path to the executable
    local csv_file="$2"   # path to the output CSV file
    local threads_list=($3)   # list of thread numbers # convert string to array
    local num_runs="$4"  # number of runs per thread count

    # Safety check
    if [[ -z "$executable" || -z "$csv_file" || -z "$threads_list" || -z "$num_runs" ]]; then
        echo "Usage: run_and_measure <executable> <csv_file> \"<threads_array>\" <num_runs>"
        return 1
    fi

    # Prepare CSV header
    touch "$csv_file"
    echo -n "run_number" > "$csv_file"
    for n in "${threads_list[@]}"; do
        echo -n ",$n" >> "$csv_file"
    done
    echo "" >> "$csv_file"

    # Loop over runs
    for run in $(seq 1 $num_runs); do
        line="$run"
        for n in "${threads_list[@]}"; do
            echo "Running: $executable with $n threads (run $run/$num_runs)"
            time_val=$(/usr/bin/time --format=%e bash -c "'$executable' -j '$n' >/dev/null 2>/dev/null" 2>&1)
            line="$line,$time_val"
        done
        echo "$line" >> "$csv_file"
    done
}

# --------------

# Example usage
EXEC="./phil" # -> write the name instead of "./..." ?
CSV="phil.csv"
THREADS="1 2 4 8"
RUNS=3

run_and_measure "$EXEC" "$CSV" "$THREADS" "$RUNS"


# Still problem with measuring time... -> i'll work on it tomorrow ...