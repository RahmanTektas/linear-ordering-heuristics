#!/bin/bash

# Runs the experiments for exercise 1.1, which includes 12 algorithms obtained by combining the two 
# pivoting rules, three neighborhoods, and two initialization methods. 
# The results are saved in a raw format in exercise1_results.txt

INSTANCE_DIR="../instances"
EXECUTABLE="../lop"
OUTPUT_FILE="../results/exercise1_results.txt"

echo "Instance Init Pivot Neighborhood Cost Time" > "$OUTPUT_FILE"

for instance_path in "$INSTANCE_DIR"/*; do
    instance_name=$(basename "$instance_path")

    for init in "--random" "--cw"; do
        for pivot in "--first" "--best"; do
            for neigh in "--transpose" "--exchange" "--insert"; do
                echo "Running: $instance_name $init $pivot $neigh"

                RESULT=$($EXECUTABLE $init $pivot $neigh -i "$instance_path")

                clean_init=${init#--}
                clean_pivot=${pivot#--}
                clean_neigh=${neigh#--}

                echo "$instance_name $clean_init $clean_pivot $clean_neigh $RESULT" >> "$OUTPUT_FILE"
            done
        done
    done
done

echo "Experiments complete. Raw data saved to $OUTPUT_FILE."