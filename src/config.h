#pragma once
#include <cstdint>
#include <string>

class Config {
private:
    std::uint32_t subnetAddress;
    std::uint32_t subnetMask;
    std::string macAddressPrefix;
    std::int64_t lifetime;
public:
    Config(const std::string& ipAddress,
            std::string  macAddress,
            std::int64_t lifetime);
    int64_t getLifetime(uint32_t ipAddress, std::string macAddress) const;
};
