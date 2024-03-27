#!/bin/bash

for i in {1..2}; do
    for file in ./benchmarks/*; do
        for parameter in entropy size depth; do
            bench=$(basename "${file%.*}")
            echo "./main $file 70000 $parameter >> ./results/$bench-$parameter-$i.csv"
            ./main $file 70000 $parameter >> "./results/$bench-$parameter-$i.csv" && wait
        done
    done
done
