// generador genera instancias validas para el primer requerimiento de experimentacion: dada una instancia, crea variaciones de
// cantidad de sensores / radios para ver como varia la performance en funcion de estos.
// Actualmente cuenta con tres instancias:
//   * 'frio': un horno frio con 30 grados en los extremos.
//   * 'calor': un horno caliente con 480 grados en los extremos.
//   * 'friocalor': un horno caliente en la parte de arriba y frio en la parte de abajo.
//
// Para crear un nuevo horno, hay que crear una funcion que dado un angulo te devuelva la temperatura externa del horno en ese angulo
//
// Mediante flags se maneja el comportamiento de todo el programa, desde cómo es la instancia hasta cómo se discretiza.
package main

import (
	"flag"
	"fmt"
	"log"
	"math"
	"os"
	"path"
)

var (
	maxAng   = flag.Int("maxAng", 120, "Máxima cantidad de ángulos")
	maxRad   = flag.Int("maxRad", 120, "Máxima cantidad de radios")
	ri       = flag.Float64("ri", 10, "Radio pared interna")
	re       = flag.Float64("re", 100, "Radio pared externa")
	isoterma = flag.Float64("iso", 500, "Valor de la isoterma")
	inst     = flag.String("inst", "frio", "Nombre de la instancia, puede ser: 'frio', 'calor', 'friocalor'")
	startAng = flag.Int("startAng", 20, "Cantidad inicial de angulos")
	startRad = flag.Int("startRad", 20, "Cantidad inicial de radios")
	inc      = flag.Int("inc", 5, "Incremento de sensores / radios por iteracion")
)

// internas es un mapa de string a funcion: Para cada instancia hay una funcion que indica que valor corresponde en el horno.
// por ejemplo, para "frio", va a retornar una funcion que para cualquier theta, devuelva 1500.
var internas = map[string]func(theta float64) float64{
	"frio":      func(theta float64) float64 { return 1500 },
	"calor":     func(theta float64) float64 { return 1500 },
	"friocalor": func(theta float64) float64 { return 1500 },
}

// externas es igual a internas, solo que para las paredes externas del horno.
// por ejemplo, para "friocalor" va a retornar una funcion que para la mitad del horno devuelva algo caliente
// y para la otra mitad algo frío.
var externas = map[string]func(theta float64) float64{
	"frio":  func(theta float64) float64 { return 30 },
	"calor": func(theta float64) float64 { return 480 },
	"friocalor": func(theta float64) float64 {
		// La mitad de este horno está muy caliente y la otra mitad muy fria
		if theta > math.Pi {
			return 480
		} else {
			return 30
		}
	},
}

func main() {
	flag.Parse()

	validateFlags()

	generarInstancia()
}

func generarInstancia() {
	incm()
	incn()
	incmn()
}

// incm incrementa los radios, dejando los sensores fijos y generando un archivo por cada incremento.
func incm() {
	m := *startRad
	i := 0
	for m < *maxRad {
		generarArchivo(m, *startAng, "m", i)
		m += 5
		i += 1
	}
}

// incn incrementa los sensores, dejando los radios fijos y genera un archivo por cada incremento.
func incn() {
	n := *startAng
	i := 0
	for n < *maxAng {
		generarArchivo(*startRad, n, "n", i)
		n += 5
		i += 1
	}
}

// incmn incrementa los radios y los sensores a la vez, generando un archivo por cada incremento.
func incmn() {
	n := *startAng
	m := *startRad
	i := 0
	for n+m < *maxRad {
		generarArchivo(m, n, "mn", i)
		m += 2
		n += 2
		i += 1
	}
}

// generarArchivo genera un archivo para un caso dado del problema.
// m representa la cantidad de radios que va a tener la instancia
// n representa la cantidad de angulos de la instancia
// prefix corresponde al prefijo del archivo, por ejemplo "radios"
// index corresponde al indice del archivo del total de archivos que se van a generar.
// ademas, usa algunas de los flags seteados.
// por ejemplo, un archivo generado podria ser frio/m/1.in
func generarArchivo(m, n int, prefix string, index int) {
	p := path.Join(*inst, prefix)
	if err := os.MkdirAll(p, 0777); err != nil {
		panic(err)
	}

	filename := path.Join(p, fmt.Sprintf("%d.in", index))

	f, err := os.Create(filename)
	if err != nil {
		panic(err)
	}
	defer f.Close()

	line1 := fmt.Sprintf("%.2f %.2f %d %d %.2f 1\n", *ri, *re, m, n, *isoterma)
	f.WriteString(line1)

	fun := internas[*inst]
	for i := 0; i < n; i++ {
		k := fun(ang(i, n))
		f.WriteString(fmt.Sprintf("%.2f ", k))
	}

	fun = externas[*inst]
	for i := 0; i < n; i++ {
		k := fun(ang(i, n))
		f.WriteString(fmt.Sprintf("%.2f ", k))
	}
	f.WriteString("\n")
}

// ang devuelve el angulo correspondiente a la iesima parte de dividir la circunferencia en n partes iguales.
// para eso, divide a la circunferencia ( 2 pi ) en n partes iguales y la multiplica por i.
func ang(i, n int) float64 {
	return float64(i) * math.Pi * 2 / float64(n)
}

// validateFlags valida que los flags de input sean validos. En caso de no serlos, muestra un error y finaliza el programa.
func validateFlags() {
	if _, ok := internas[*inst]; !ok {
		log.Println("inst debe ser una de: 'frio', 'calor', 'friocalor'")
		os.Exit(1)
	}

	if *maxAng <= 0 || *maxRad <= 0 {
		log.Println("La cantidad de sensores / radios debe ser positiva")
		os.Exit(1)
	}

	if *ri < 0 || *re < 0 {
		log.Println("Los radios de las paredes no pueden ser negativos")
		os.Exit(1)
	}

	if *isoterma < 0 {
		log.Println("La isoterma no puede ser negativa")
		os.Exit(1)
	}
}
