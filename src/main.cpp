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
        logger.info("load");
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
