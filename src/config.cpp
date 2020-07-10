#include <string>
#include <cstdint>
#include <regex>
#include <utility>
#include "config.h"

std::regex cidr(R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})/(\d{1,2}))");

Config::Config(const std::string& ipAddress,
        std::string  macAddress,
        std::int64_t lifetime):macAddressPrefix(std::move(macAddress)),lifetime(lifetime) {
    std::smatch match;
    if (std::regex_match(ipAddress, match, cidr)) {
        this->subnetAddress = (
                (std::atoi(match[1].str().c_str()) << 24) +
                (std::atoi(match[2].str().c_str()) << 16) +
                (std::atoi(match[3].str().c_str()) << 8) +
                (std::atoi(match[4].str().c_str())));
        this->subnetMask = ~((1 << (32 - std::atoi(match[5].str().c_str()))) - 1);
    } else {
        this->subnetAddress = 0;
        this->subnetMask = 0;
    }
}

int64_t Config::getLifetime(uint32_t ipAddress, std::string macAddress) const {
    if ((this->subnetAddress ^ ipAddress) & this->subnetMask) {
        return -1;
    }
    if (macAddress.find(this->macAddressPrefix) != 0) {
        return -1;
    }
    return this->lifetime;
}
