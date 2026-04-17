#!/bin/bash

# This scripts runs all the algorithms for both exercises to save the results in .dat files, then these
# files are gonna be used by the R scripts to do the statistical tests.

INSTANCE_DIR="../instances"
EXECUTABLE="../lop"
RESULTS_DIR="../results"

# best known values
> "$RESULTS_DIR/best_known.dat"

# # Exercise 1.1: 12 algorithms
# > "$RESULTS_DIR/lop_first_transpose_random.dat"
# > "$RESULTS_DIR/lop_first_transpose_cw.dat"
# > "$RESULTS_DIR/lop_first_exchange_random.dat"
# > "$RESULTS_DIR/lop_first_exchange_cw.dat"
# > "$RESULTS_DIR/lop_first_insert_random.dat"
# > "$RESULTS_DIR/lop_first_insert_cw.dat"

# > "$RESULTS_DIR/lop_best_transpose_random.dat"
# > "$RESULTS_DIR/lop_best_transpose_cw.dat"
# > "$RESULTS_DIR/lop_best_exchange_random.dat"
# > "$RESULTS_DIR/lop_best_exchange_cw.dat"
# > "$RESULTS_DIR/lop_best_insert_random.dat"
# > "$RESULTS_DIR/lop_best_insert_cw.dat"

# # Exercise 1.2: 2 VND algorithms
# > "$RESULTS_DIR/vnd_tei.dat"
# > "$RESULTS_DIR/vnd_tie.dat"

for instance_path in "$INSTANCE_DIR"/*; do
    instance_name=$(basename "$instance_path")

    BEST=$(awk -v name="$instance_name" '$1 == name {print $2}' ../best_known/best_known.txt)
    echo "$BEST" >> "$RESULTS_DIR/best_known.dat"

    # # Exercise 1.1
    # RESULT=$($EXECUTABLE --random --first --transpose -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_first_transpose_random.dat"

    # RESULT=$($EXECUTABLE --cw --first --transpose -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_first_transpose_cw.dat"

    # RESULT=$($EXECUTABLE --random --first --exchange -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_first_exchange_random.dat"

    # RESULT=$($EXECUTABLE --cw --first --exchange -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_first_exchange_cw.dat"

    # RESULT=$($EXECUTABLE --random --first --insert -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_first_insert_random.dat"

    # RESULT=$($EXECUTABLE --cw --first --insert -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_first_insert_cw.dat"

    # RESULT=$($EXECUTABLE --random --best --transpose -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_best_transpose_random.dat"

    # RESULT=$($EXECUTABLE --cw --best --transpose -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_best_transpose_cw.dat"

    # RESULT=$($EXECUTABLE --random --best --exchange -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_best_exchange_random.dat"

    # RESULT=$($EXECUTABLE --cw --best --exchange -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_best_exchange_cw.dat"

    # RESULT=$($EXECUTABLE --random --best --insert -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_best_insert_random.dat"

    # RESULT=$($EXECUTABLE --cw --best --insert -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/lop_best_insert_cw.dat"

    # # Exercise 1.2
    # RESULT=$($EXECUTABLE --cw --vnd-tei -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/vnd_tei.dat"

    # RESULT=$($EXECUTABLE --cw --vnd-tie -i "$instance_path")
    # echo "$RESULT" | awk '{print $1}' >> "$RESULTS_DIR/vnd_tie.dat"
done