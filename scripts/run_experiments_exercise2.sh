#!/bin/bash

INSTANCE_DIR="../instances"
EXECUTABLE="../lop"
OUTPUT_FILE="../results/exercise2_vnd_results.txt"

echo "Instance Method Cost Time" > "$OUTPUT_FILE"

for instance_path in "$INSTANCE_DIR"/*; do
    instance_name=$(basename "$instance_path")

    for method in "--vnd-tei" "--vnd-tie"; do
        echo "Running: $instance_name $method"

        RESULT=$($EXECUTABLE --cw $method -i "$instance_path")

        clean_method=${method#--}
        echo "$instance_name $clean_method $RESULT" >> "$OUTPUT_FILE"
    done
done

echo "VND experiments complete. Raw data saved to $OUTPUT_FILE."