package main

import (
	"flag"
	"fmt"
	"os"
)

var (
	ri       = flag.Float64("ri", 10, "Radio pared interna")
	re       = flag.Float64("re", 100, "Radio pared externa")
	isoterma = flag.Float64("iso", 500, "Valor de la isoterma")
	n        = flag.Int("n", 10, "Cantidad de angulos")
	m        = flag.Int("m", 10, "Cantidad de radios")
	v        = flag.Float64("var", 5, "Variacion de los valores de los sensores por iteracion")
)

func main() {
	flag.Parse()

	generarInstancias()
}

func generarInstancias() {
	for i := 0; i < 20; i++ {
		generarArchivo(*m+2*i, 1+i*5, 0, *v, fmt.Sprintf("calentando%d.in", i))
		//generarArchivo(1500, -*v, "enfriando.in")
	}
}

func generarArchivo(m, ninsts int, inicial, v float64, nombre string) {
	f, err := os.Create(nombre)
	if err != nil {
		panic(err)
	}
	defer f.Close()

	line1 := fmt.Sprintf("%.2f %.2f %d %d %.2f %d\n", *ri, *re, m, *n, *isoterma, ninsts)
	f.WriteString(line1)

	k := inicial
	for j := 0; j < ninsts; j++ {
		for i := 0; i < *n; i++ { // Las paredes internas siempre seran 1500
			f.WriteString(fmt.Sprintf("%.2f ", 1500.0))
		}

		for i := 0; i < *n; i++ {
			f.WriteString(fmt.Sprintf("%.2f ", k))
		}
		f.WriteString("\n")
		k += v
	}
}
