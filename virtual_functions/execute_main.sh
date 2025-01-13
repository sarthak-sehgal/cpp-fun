#!/bin/bash

# The name of the program you want to run
program="./main"

# Loop to run the program 1000 times
for i in {1..100}
do
    echo "Running iteration $i"
    sleep 3
    $program
done

echo "Finished running the program 100 times."
