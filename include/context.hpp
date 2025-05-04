#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <cstdint>

#include <libudev.h>

#include "device.hpp"
#include "monitor.hpp"

namespace Udev {

    /// Udev library context
    class Context {

        public:

            /// Create a new udev context
            Context();

            /// Create a new udev context with the given udev context,
            /// increasing the reference count
            Context(struct udev* ctx);


            /// Get the contained udev context, increasing the reference count
            struct udev* get();

            /// Get the contained udev context
            struct udev* _get();


            /// Get a udev device from a syspath
            Device getDeviceFromSyspath(const std::string& syspath);

            /// Get a udev device from a devnum
            enum DevnumType : uint8_t { BLOCK, CHAR };
            Device getDeviceFromDevnum(DevnumType type, uint64_t devnum);

            /// Get a udev device via subsystem and sysname
            Device getDeviceFromSubsystemSysname(const std::string& subsystem, const std::string& sysname);

            /// Get a udev device from its device id
            Device getDeviceFromId(const std::string& id);

            /// Get a udev device from the environment
            Device getDeviceFromEnv();


            /// Create a monitor for the udev context
            enum MonitorType : uint8_t { UDEV, KERNEL };
            Monitor createMonitor(MonitorType type);


            /// Decrease the reference count of the udev context
            ~Context();

            Context(const Context &) = delete;
            Context(Context &&) = delete;
            Context &operator=(const Context &) = delete;
            Context &operator=(Context &&) = delete;

        private:
            struct udev* ctx;

    };

}

#endif // CONTEXT_HPP
