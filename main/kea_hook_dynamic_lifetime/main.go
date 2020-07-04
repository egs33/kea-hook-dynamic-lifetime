package main

import "C"

import (
	"net"
	"strconv"
	"strings"
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

//export getLifetime
func getLifetime(ipAddress *C.char, macAddress *C.char) C.int {
	ip := net.ParseIP(C.GoString(ipAddress))
	if ip == nil {
		return 0
	}
	mac := C.GoString(macAddress)

	for _, c := range loadedConfigs {
		if c.ipAddress != "" {
			_, cidr, err := net.ParseCIDR(c.ipAddress)
			if err != nil {
				continue
			}
			if !cidr.Contains(ip) {
				continue
			}
		}
		if c.macAddress != "" && !strings.HasPrefix(mac, c.macAddress) {
			continue
		}
		return C.int(c.lifetime)
	}

	return 0
}

func main() {
}
