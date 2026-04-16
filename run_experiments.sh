#!/bin/bash

# Define the paths
INSTANCE_DIR="./instances"
EXECUTABLE="./lop" # Or ./lop11 if you renamed it
OUTPUT_FILE="exercise1_results.txt"

# Clear the output file and add a header (using spaces to make Python parsing easier)
echo "Instance Init Pivot Neighborhood Cost Time" > $OUTPUT_FILE

# Loop through every instance file in the directory
for instance_path in $INSTANCE_DIR/*; do
    instance_name=$(basename "$instance_path")
    
    # Loop using the exact new flags for Initializations
    for init in "--random" "--cw"; do
        # Loop using the exact new flags for Pivoting Rules
        for pivot in "--first" "--best"; do
            # Loop using the exact new flags for Neighborhoods
            for neigh in "--transpose" "--exchange" "--insert"; do
                
                echo "Running: $instance_name $init $pivot $neigh"
                
                # Execute the program using the exact command line format the professor asked for
                RESULT=$($EXECUTABLE $init $pivot $neigh -i "$instance_path")
                
                # For our simple.txt file, we want to remove the "--" prefix 
                # so the text file just says "cw best insert" instead of "--cw --best --insert"
                clean_init=${init#--}
                clean_pivot=${pivot#--}
                clean_neigh=${neigh#--}
                
                # Save the result to our raw data file
                echo "$instance_name $clean_init $clean_pivot $clean_neigh $RESULT" >> $OUTPUT_FILE
            done
        done
    done
done

echo "Experiments complete. Raw data saved to $OUTPUT_FILE."