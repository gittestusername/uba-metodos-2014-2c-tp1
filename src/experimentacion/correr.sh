#!/bin/bash

TIPOS=(frio calor friocalor)
VARIACIONES=(m n mn)

# Generar todos los tipos
for t in ${TIPOS[*]}
do
	go run generador.go -inst="$t"
done

# Para cada variacion, correr 10 veces, numerarlos como metric1 .. metric10
for t in ${TIPOS[*]}
do
	for v in ${VARIACIONES[*]}
	do
		for i in $(seq 1 10)
		do
			echo "Corriendo $t/$v, corrida $i"
			go run runner.go -dir="$t/$v" | sort -n > $t/$v/metric$i
		done
	done
done
