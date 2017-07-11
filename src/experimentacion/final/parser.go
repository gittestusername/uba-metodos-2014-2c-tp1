package main

import (
	"fmt"
	"io"
)

func main() {
	cycles := make(map[int64]int64)
	ninst := make(map[int64]int64)

	for {
		var m, n, mn, cyc int64
		_, err := fmt.Scanln(&m, &n, &mn, &cyc)
		if err != nil {
			if err != io.EOF {
				panic(err)
			}
			break
		}

		cycles[m] += cyc
		ninst[m] += 1

	}

	for k, v := range ninst {
		fmt.Printf("%d %d %d\n", k, v, cycles[k])
	}
}
