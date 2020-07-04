#include <iostream>
#include <fstream>
#include <hooks/hooks.h>
#include <dhcpsrv/lease.h>
#include <dhcp/hwaddr.h>
#include <log/message_initializer.h>
#include <log/macros.h>
#include "../go_dist/libdynamic_lifetime_go.h"

isc::log::Logger logger("dynamic_lifetime");

typedef struct {
    char* ipAddress;
    char* macAddress;
    int lifetime;
} config;

extern "C" {
    int version() {
        int v = keaVersion();
        if (v < 0) {
            return KEA_HOOKS_VERSION;
        }
        return v;
    }

    int load(isc::hooks::LibraryHandle& handle) {
        logger.info("load");
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
            char* ipAddress = v->contains("ip-address") ?
                    const_cast<char *>(v->get("ip-address")->stringValue().c_str()) :
                              nullptr;
            char* macAddress = v->contains("mac-address") ?
                    const_cast<char *>(v->get("mac-address")->stringValue().c_str()) :
                               nullptr;
            if (!v->contains("valid-lifetime")) {
                logger.warn("option 'valid-lifetime' required");
                continue;
            }
            int lifetime = v->get("valid-lifetime")->intValue();
            addConfig(ipAddress, macAddress, lifetime);
        }
        return 0;
    }

    int unload() {
        logger.info("unload");
        return 0;
    }

    int lease4_select(isc::hooks::CalloutHandle& handle) {
        logger.info("lease4_select");
        return 0;
    }

    int lease4_renew(isc::hooks::CalloutHandle& handle) {
        logger.info("lease4_renew");
        return 0;
    }
}
