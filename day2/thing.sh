#!/bin/bash

start_time=$(date +%s%N)

# Run the specified command
"$@"

end_time=$(date +%s%N)

# Calculate the elapsed time in milliseconds
elapsed_time=$(( (end_time - start_time) / 1000000 ))

echo "Elapsed Time: ${elapsed_time} milliseconds"

