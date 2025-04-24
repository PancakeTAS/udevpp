#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <libudev.h>

namespace Udev {

    /// Udev device
    class Device {

        friend class Context;
        friend class Monitor;

        public:

            /// Create a new udev device with the given udev device, increasing the reference count
            Device(struct udev_device* dev);


            /// Get the contained udev device, increasing the reference count
            struct udev_device* get();

            /// Get the contained udev device
            struct udev_device* _get();


            /// Get the parent device
            Device getParent();

            /// Get the parent device with the given subsystem and device type
            Device getParentWithSubsystemDevtype(std::string_view subsystem, std::string_view devtype);


            /// Get the device path (e.g. /devices/pci0000:00/0000:00:03.1/0000:0a:00.0/drm/card1)
            std::string getDevpath();

            /// Get the device subsystem (e.g. drm)
            std::string getSubsystem();

            /// Get the device type (e.g. drm_minor)
            std::string getDevtype();

            /// Get the system path (e.g. /sys/devices/pci0000:00/0000:00:03.1/0000:0a:00.0/drm/card1)
            std::string getSyspath();

            /// Get the system name (e.g. card1)
            std::string getSysname();

            /// Get the system number (e.g. 1)
            std::string getSysnum();

            /// Get the device node (e.g. /dev/dri/card1)
            std::string getDevnode();

            /// Get a specific property value (e.g. MAJOR=226)
            std::string getProperty(std::string_view key);

            /// Get the device driver (e.g. nvidia)
            std::string getDriver();

            /// Get the device number (e.g. 57857)
            uint64_t getDevnum();

            /// Get the action
            std::string getAction();

            /// Get the sequence number (e.g. 0)
            uint64_t getSeqnum();

            /// Get the usec since initialized (e.g. 5421328295 usec)
            uint64_t getUsecSinceInitialized();

            /// Is the device initialized
            bool isInitialized();

            /// Get a sysattr value (e.g. power_state = D0)
            std::string getSysattr(std::string_view sysattr);


            /// Check if the device has a specific tag
            bool hasTag(std::string_view tag);

            /// Check if the device has a current tag
            bool hasCurrentTag(std::string_view tag);


            /// Set a sysattr value
            void setSysattr(std::string_view sysattr, std::string_view value);


            /// Get all links pointing to this device
            std::vector<std::string> getDevlinks();

            /// Get all tags of this device
            std::vector<std::string> getTags();

            /// Get all current tags of this device
            std::vector<std::string> getCurrentTags();


            /// Get all properties of this device
            std::unordered_map<std::string, std::string> getProperties();

            /// Get all system attributes of this device
            std::unordered_map<std::string, std::string> getSysattrs();


            /// Decrease the reference count of the udev device
            ~Device();

        private:
            struct udev_device* dev;

            Device(struct udev_device* dev, bool ref);

    };

}

#endif // DEVICE_HPP
