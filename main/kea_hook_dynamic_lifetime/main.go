package main

import "C"

import (
	"strconv"
)

var targetVersion string

type config struct {
	ipAddress  string
	macAddress string
	lifetime   int
}

var loadedConfigs []config

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

//export addConfig
func addConfig(ipAddress *C.char, macAddress *C.char, lifetime C.int) {
	goConfig := config{
		ipAddress:  C.GoString(ipAddress),
		macAddress: C.GoString(macAddress),
		lifetime:   int(lifetime),
	}
	loadedConfigs = append(loadedConfigs, goConfig)
}

func main() {
}
