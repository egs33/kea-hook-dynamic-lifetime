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
- cmake
- kea-dev
- libboost-dev

### Build shared library
Run
```
$ cmake .
$ make
```

`./libdynamic_lifetime.so` is builded.
