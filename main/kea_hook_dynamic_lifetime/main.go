package main

import "C"

import (
	"strconv"
)

var targetVersion string

//export keaVersion
func keaVersion() C.int {
	if targetVersion == "" {
		return C.int(-1)
	}
	i, err := strconv.Atoi(targetVersion)
	if err != nil {
		return C.int(-1)
	}
	return C.int(i)
}

func main() {
}
