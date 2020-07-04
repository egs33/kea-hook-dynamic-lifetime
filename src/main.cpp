#include <iostream>
#include <fstream>
#include <hooks/hooks.h>
#include <dhcpsrv/lease.h>
#include <dhcp/hwaddr.h>
#include <log/message_initializer.h>
#include <log/macros.h>
#include "../go_dist/libdynamic_lifetime_go.h"

isc::log::Logger logger("dynamic_lifetime");

extern "C" {
    int version() {
        int v = keaVersion();
        if (v < 0) {
            return KEA_HOOKS_VERSION;
        }
        return v;
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
            int lifetime = v->get("valid-lifetime")->intValue();
            addConfig(const_cast<char *>(ipAddress.c_str()),
                    const_cast<char *>(macAddress.c_str()),
                    lifetime);
        }
        return 0;
    }

    void updateLifetime(isc::hooks::CalloutHandle& handle) {
        isc::dhcp::Lease4Ptr l4p;
        handle.getArgument("lease4", l4p);
        auto macAddress = l4p->hwaddr_->toText(false);
        int lifetime = getLifetime(
                const_cast<char *>(l4p->addr_.toText().c_str()),
                const_cast<char *>(macAddress.c_str()));
        if (lifetime > 0) {
            logger.info(("lifetime:" + std::to_string(lifetime) + ", hw-address:" + macAddress).c_str());
            l4p->valid_lft_ = lifetime;
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
