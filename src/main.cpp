#include <hooks/hooks.h>
#include <dhcpsrv/lease.h>
#include <dhcp/hwaddr.h>
#include <log/macros.h>
#include "config.h"

isc::log::Logger logger("dynamic_lifetime");

std::vector<Config*> configs;

extern "C" {
    int version() {
        return KEA_HOOKS_VERSION;
    }

    int load(isc::hooks::LibraryHandle& handle) {
        auto options = handle.getParameter("options");
        if (!options) {
            logger.warn("parameter 'options' not found");
            return 0;
        }
        if (options->getType() != isc::data::Element::list) {
            logger.error("parameter 'options' must be array");
            return 1;
        }
        auto listValues = options->listValue();
        for (const auto& v : listValues) {
            std::string ipAddress = v->contains("ip-address") ?
                    v->get("ip-address")->stringValue() :
                    "";
            std::string macAddress = v->contains("mac-address") ?
                    v->get("mac-address")->stringValue() :
                    "";
            if (!v->contains("valid-lifetime")) {
                logger.warn("option 'valid-lifetime' required");
                continue;
            }
            int64_t lifetime = v->get("valid-lifetime")->intValue();
            configs.push_back(new Config(ipAddress, macAddress, lifetime));
        }
        return 0;
    }

    void updateLifetime(isc::hooks::CalloutHandle& handle) {
        isc::dhcp::Lease4Ptr l4p;
        handle.getArgument("lease4", l4p);
        if (!l4p->addr_.isV4()) {
            return;
        }
        auto macAddress = l4p->hwaddr_->toText(false);
        auto ipAddress = l4p->addr_.toUint32();
        for(auto &&c:configs) {
            int32_t lifetime = c->getLifetime(ipAddress, macAddress);
            if (lifetime > 0) {
                logger.info(("lifetime:" + std::to_string(lifetime) + ", hw-address:" + macAddress).c_str());
                l4p->valid_lft_ = lifetime;
                return;
            }
        }
    }

    int lease4_select(isc::hooks::CalloutHandle& handle) {
        updateLifetime(handle);
        return 0;
    }

    int lease4_renew(isc::hooks::CalloutHandle& handle) {
        updateLifetime(handle);
        return 0;
    }
}
