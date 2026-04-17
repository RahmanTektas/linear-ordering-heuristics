#!/bin/bash

INSTANCE_DIR="../instances"
EXECUTABLE="../lop"

# best known values
> best_known.dat

# Exercise 1.1: 12 algorithms
> lop_first_transpose_random.dat
> lop_first_transpose_cw.dat
> lop_first_exchange_random.dat
> lop_first_exchange_cw.dat
> lop_first_insert_random.dat
> lop_first_insert_cw.dat

> lop_best_transpose_random.dat
> lop_best_transpose_cw.dat
> lop_best_exchange_random.dat
> lop_best_exchange_cw.dat
> lop_best_insert_random.dat
> lop_best_insert_cw.dat

# Exercise 1.2: 2 VND algorithms
> vnd_tei.dat
> vnd_tie.dat

for instance_path in "$INSTANCE_DIR"/*; do
    instance_name=$(basename "$instance_path")

    BEST=$(awk -v name="$instance_name" '$1 == name {print $2}' ../best_known/best_known.txt)
    echo "$BEST" >> best_known.dat

    # Exercise 1.1
    RESULT=$($EXECUTABLE --random --first --transpose -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_first_transpose_random.dat

    RESULT=$($EXECUTABLE --cw --first --transpose -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_first_transpose_cw.dat

    RESULT=$($EXECUTABLE --random --first --exchange -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_first_exchange_random.dat

    RESULT=$($EXECUTABLE --cw --first --exchange -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_first_exchange_cw.dat

    RESULT=$($EXECUTABLE --random --first --insert -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_first_insert_random.dat

    RESULT=$($EXECUTABLE --cw --first --insert -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_first_insert_cw.dat

    RESULT=$($EXECUTABLE --random --best --transpose -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_best_transpose_random.dat

    RESULT=$($EXECUTABLE --cw --best --transpose -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_best_transpose_cw.dat

    RESULT=$($EXECUTABLE --random --best --exchange -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_best_exchange_random.dat

    RESULT=$($EXECUTABLE --cw --best --exchange -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_best_exchange_cw.dat

    RESULT=$($EXECUTABLE --random --best --insert -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_best_insert_random.dat

    RESULT=$($EXECUTABLE --cw --best --insert -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> lop_best_insert_cw.dat

    # Exercise 1.2
    RESULT=$($EXECUTABLE --cw --vnd-tei -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> vnd_tei.dat

    RESULT=$($EXECUTABLE --cw --vnd-tie -i "$instance_path")
    echo "$RESULT" | awk '{print $1}' >> vnd_tie.dat
done