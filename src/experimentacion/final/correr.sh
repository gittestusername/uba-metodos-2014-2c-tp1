#!/bin/bash


for j in $(seq 1 10)
do
    rm -f metricLU$j
    for i in $(seq 0 19)
    do
        echo "Corriendo LU calentando${i}.in corrida $j"
        ./tp calentando${i}.in tmp.out 1 tmp.isoterma >> metricLU$j
    done
    rm -f metricG$j
    for i in $(seq 0 19)
    do
        echo "Corriendo Gauss calentando${i}.in corrida $j"
        ./tp calentando${i}.in tmp.out 0 tmp.isoterma >> metricG$j
    done
done

./merger.py metricLU metricLU
./merger.py metricG metricG
go run parser.go < metricLU | sort -n > metricLUfinal
go run parser.go < metricG | sort -n > metricGfinal
