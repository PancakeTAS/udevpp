#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <optional>

#include <libudev.h>

#include "device.hpp"

namespace Udev {

    /// Udev library context
    class Monitor {

        friend class Context;

        public:

            /// Create a new udev monitor with the given udev monitor, increasing the reference count
            Monitor(struct udev_monitor* mon);


            /// Get the contained udev monitor, increasing the reference count
            struct udev_monitor* get();

            /// Get the contained udev monitor
            struct udev_monitor* _get();


            /// Match for subsystem and device type
            void filterSubsystemDevtype(const std::string& subsystem, std::optional<std::string> devtype);

            /// Match for a tag
            void filterTag(const std::string& tag);


            /// Update the filter
            void updateFilter();

            /// Remove the filter
            void removeFilter();


            /// Enable receiving events
            void enableReceiving();

            /// Set the receive buffer size
            void setReceiveBufferSize(int size);

            /// Check if a device can be received
            bool canReceive();

            /// Receive a device from the udev monitor
            Device receive();


            /// Decrease the reference count of the udev monitor
            ~Monitor();

            Monitor(const Monitor &) = delete;
            Monitor(Monitor &&) = delete;
            Monitor &operator=(const Monitor &) = delete;
            Monitor &operator=(Monitor &&) = delete;

        private:
            struct udev_monitor* mon;
            int fd;

            Monitor(struct udev_monitor* mon, bool ref);

    };

}

#endif // MONITOR_HPP
