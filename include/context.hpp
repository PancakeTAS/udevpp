#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cstdint>
#include <string_view>

#include <libudev.h>

#include "device.hpp"
#include "monitor.hpp"

namespace Udev {

    /// Udev library context
    class Context {

        public:

            /// Create a new udev context
            Context();

            /// Create a new udev context with the given udev context, increasing the reference count
            Context(struct udev* ctx);


            /// Get the contained udev context, increasing the reference count
            struct udev* get();

            /// Get the contained udev context
            struct udev* _get();


            /// Get a udev device from a syspath
            Device getDeviceFromSyspath(std::string_view syspath);

            /// Get a udev device from a devnum
            enum DevnumType { BLOCK, CHAR };
            Device getDeviceFromDevnum(DevnumType type, uint64_t devnum);

            /// Get a udev device via subsystem and sysname
            Device getDeviceFromSubsystemSysname(std::string_view subsystem, std::string_view sysname);

            /// Get a udev device from its device id
            Device getDeviceFromId(std::string_view id);

            /// Get a udev device from the environment
            Device getDeviceFromEnv();


            /// Create a monitor for the udev context
            enum MonitorType { UDEV, KERNEL };
            Monitor createMonitor(MonitorType type);


            /// Decrease the reference count of the udev context
            ~Context();

        private:
            struct udev* ctx;

    };

}

#endif // CONTEXT_HPP
