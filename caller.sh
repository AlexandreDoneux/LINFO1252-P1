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

    # Create results directory if it doesn't exist
    mkdir -p "$(dirname "$csv_file")"

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
            time_val=$(/usr/bin/time --format=%e bash -c "'$executable' '$n' >/dev/null 2>/dev/null" 2>&1)
            line="$line,$time_val"
        done
        echo "$line" >> "$csv_file"
    done
}

# --------------
# Main script execution
# --------------

THREADS="2 4"
THREADS_SPINLOCK="1 2 4"
#THREADS="2 4 8 16 32" -> put back for final solution
#THREADS_SPINLOCK="1 2 4 8 16 32" -> put back for final solution
RUNS=2 # put five for final solution

run_and_measure "./src/phil" "results/phil.csv" "$THREADS" "$RUNS"
run_and_measure "./src/prod" "results/prod.csv" "$THREADS" "$RUNS"
run_and_measure "./src/read" "results/read.csv" "$THREADS" "$RUNS"



