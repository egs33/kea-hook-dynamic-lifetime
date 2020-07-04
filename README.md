# kea-hook-dynamic-lifetime
Kea dhcp hook for setting lifetime by IP address and MAC address

## Usage
example config:
```jsonc
    "hooks-libraries": [
        {
            "library": "/usr/local/lib/kea/hooks/libdynamic_lifetime.so",
            "parameters": {
                "options": [
                    {
                        "mac-address": "00:11:22:33:44:55",
                        "valid-lifetime": 3600
                    }, {
                        "mac-address": "00:11:22",
                        "valid-lifetime": 7200
                    }, {
                        "ip-address": "192.168.10.0/24",
                        "valid-lifetime": 72000
                   }
                ]
            }
        }
    ],
```
This config is
- Set lifetime to `3600` if MAC address is `00:11:22:33:44:55`
- Set lifetime to `7200` if MAC address starts with `00:11:22`
- Set lifetime to `72000` if leased IP address is in `192.168.10.0/24`
- Otherwise, default lifetime is used.

The higher the setting, the higher the priority.
The IP address parameter must be CIDR form.

These config override other `valid-lifetime` config
(ex. global config, subnet config).


## Build
### Required
- golang
- cmake
- kea-dev
- libboost-dev

### 1. Build go source (make static library)
Run
```
$ go build -buildmode=c-archive -o go_dist/libdynamic_lifetime_go.a ./main/kea_hook_dynamic_lifetime
```

If you require custom [version](https://fossies.org/dox/kea-1.6.2/hooksdgDevelopersGuide.html#hooksdgVersionFunction),
add `-ldflags="-X main.targetVersion=VERSION"` to command.

### 2. Build shared library
Run
```
$ cmake .
$ make
```

`./libdynamic_lifetime.so` is builded.
