// runner ejecuta el programa pasado como parametro sobre los archivos de un caso de prueba y genera un output para cada archivo, por stdout de la forma nombreArchivo m n m+n ciclos
// por ejemplo, correr go run runner.go frio/m generaria para cada archivo en la carpeta frio/m cuántos ciclos insumió:
// 0.in 10 20 30 1441515151
// 1.in 10 30 40 4134324214
// 2.in 10 40 50 3423423526
// etc.
package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

var (
	dir = flag.String("dir", "frio/m", "Carpeta en donde obtener los archivos .in")
)

func main() {
	flag.Parse()
	validateFlags()

	glob := filepath.Join(*dir, "*.in")
	files, err := filepath.Glob(glob)
	if err != nil {
		panic(err)
	}

	for _, v := range files {
		processFile(v)
	}

}

func processFile(v string) {
	// Ejecutamos tp con parametros: v, v.out (reemplazando el .in), 0, v.isoterma
	output := strings.Replace(v, ".in", ".out", 1)
	isoterma := strings.Replace(v, ".in", ".isoterma", 1)

	tpCmd := exec.Command("./tp", v, output, "0", isoterma)
	tpOut, err := tpCmd.Output()
	if err != nil {
		panic(err)
	}

	fmt.Println(string(tpOut))
}

func validateFlags() {
	if ok, err := exists(*dir); err != nil {
		panic(err)
	} else if !ok {
		log.Printf("La carpeta %s no existe", *dir)
		os.Exit(1)
	}
}

func exists(path string) (bool, error) {
	_, err := os.Stat(path)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return false, err
}
